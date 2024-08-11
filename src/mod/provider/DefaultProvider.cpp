#include "DefaultProvider.h"
#include "../PPMessages.h"
#include <fstream>

DefaultProvider::DefaultProvider(mcpm::PurePerms* _plugin)
{
  plugin = _plugin;
  ifstream in("plugins/PurePerms/ranks.yaml"),in1("plugins/PurePerms/players.yaml");
  if (in.is_open() & in1.is_open())
  {
    in.close();
    in1.close();
    groups = YAML::LoadFile("plugins/PurePerms/ranks.yaml");
    if (groups.IsNull())
    {
        //throw plugin->getMessage("logger_messages.YAMLProvider_InvalidGroupsSettings");
    }
    players = YAML::LoadFile("plugins/PurePerms/players.yaml");
  }
  else
  {
    in.close();
    in1.close();

    // Добавляем группу "Guest"
    YAML::Node guest;
    guest["alias"] = "gst";
    guest["isDefault"] = true;
    guest["inheritance"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность

    YAML::Node guestPermissions;
    guestPermissions.push_back("-essentials.kit");
    guestPermissions.push_back("-essentials.kit.other");
    guestPermissions.push_back("-pocketmine.command.me");
    guestPermissions.push_back("pchat.colored.format");
    guestPermissions.push_back("pchat.colored.nametag");
    guestPermissions.push_back("pocketmine.command.list");
    guestPermissions.push_back("pperms.command.ppinfo");
    guest["permissions"] = guestPermissions;
    
    guest["worlds"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность

    groups["Guest"] = guest;

    // Добавляем группу "Admin"
    YAML::Node admin;
    admin["alias"] = "adm";
    admin["inheritance"].push_back("Guest");

    YAML::Node adminPermissions;
    adminPermissions.push_back("essentials.gamemode");
    adminPermissions.push_back("pocketmine.broadcast");
    adminPermissions.push_back("pocketmine.command.gamemode");
    adminPermissions.push_back("pocketmine.command.give");
    adminPermissions.push_back("pocketmine.command.kick");
    adminPermissions.push_back("pocketmine.command.teleport");
    adminPermissions.push_back("pocketmine.command.time");
    admin["permissions"] = adminPermissions;
    
    admin["worlds"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность

    groups["Admin"] = admin;

    // Добавляем группу "Owner"
    YAML::Node owner;
    owner["alias"] = "owr";
    owner["inheritance"].push_back("Admin");

    YAML::Node ownerPermissions;
    ownerPermissions.push_back("essentials");
    ownerPermissions.push_back("pocketmine.command");
    ownerPermissions.push_back("pperms.command");
    owner["permissions"] = ownerPermissions;
    
    owner["worlds"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность

    groups["Owner"] = owner;

    // Добавляем группу "OP"
    YAML::Node op;
    op["alias"] = "op";
    op["inheritance"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность

    YAML::Node opPermissions;
    opPermissions.push_back("*");
    op["permissions"] = opPermissions;

    op["worlds"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность

    groups["OP"] = op;

    // Сохраняем в файл
    std::ofstream fout("plugins/PurePerms/ranks.yaml");
    fout << groups;
    fout.close();
    YAML::Node user;
    user["group"] = "Guest";
    user["permissions"] = YAML::Node(YAML::NodeType::Sequence);  // Пустая последовательность для permissions
    user["worlds"] = YAML::Node(YAML::NodeType::Sequence);       // Пустая последовательность для worlds

    // Вставляем в корневой узел
    players["TheMrEnderBro"] = user;
    fout.open("plugins/PurePerms/players.yaml");
    fout << players;
    fout.close();
  }
}

YAML::Node DefaultProvider::getGroupData(PPGroup group)
{
  std::string groupName = group.getName();
  YAML::Node groupsData = getGroupsData();

        // Проверка наличия данных для группы
  if (!groupsData[groupName] || !groupsData[groupName].IsMap()) {
      return YAML::Node(YAML::NodeType::Undefined); // Возвращаем неопределенный узел
  }

  return groupsData[groupName];
}

tuple<string,vector<string>,YAML::Node,int> DefaultProvider::getPlayerData(Player* player)
{
  std::string userName = toLowerCase(player->getRealName());

        // Проверяем наличие данных для игрока
        if (!players[userName]) {
            // Возвращаем дефолтные данные
            YAML::Node defaultData;
            defaultData["group"] = "player";
            defaultData["permissions"] = YAML::Node(YAML::NodeType::Sequence);
            defaultData["worlds"] = YAML::Node(YAML::NodeType::Sequence);
            defaultData["time"] = -1;
            tuple<string,vector<string>,YAML::Node,int> t = make_tuple<string,vector<string>,YAML::Node,int>("player",{},YAML::Node(YAML::NodeType::Undefined),-1);
            return t;
        }
        tuple<string,vector<string>,YAML::Node,int> t = make_tuple<string,vector<string>,YAML::Node,int>(players[userName]["group"].as<string>(),players[userName]["permissions"].as<vector<string>>(),players[userName]["worlds"],players[userName]["time"].as<int>());
        return t;
}

optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> DefaultProvider::getUsers()
{
 unordered_map<string, tuple<string, vector<string>, YAML::Node, int>> users;

        // Проверка, если данные отсутствуют
        if (players.IsNull() || players.size() == 0) 
        {
            return nullopt;  // Возвращаем std::nullopt, если данных нет
        }

        // Обход всех игроков в YAML файле
        for (auto it = players.begin(); it != players.end(); ++it) {
            string username = it->first.as<string>();

            // Извлекаем данные для каждого игрока
            YAML::Node userData = it->second;
            string group = userData["group"].as<string>("player");
            vector<string> permissions = userData["permissions"] ? userData["permissions"].as<vector<string>>() : vector<string>();
            YAML::Node worlds = userData["worlds"] ? userData["worlds"] : YAML::Node(YAML::NodeType::Sequence);
            int time = userData["time"] ? userData["time"].as<int>() : -1;

            // Добавляем данные в карту
            users[username] = make_tuple(group, permissions, worlds, time);
        }

        return users;
}

void DefaultProvider::setGroupData(PPGroup& group, tuple<string,vector<string>,YAML::Node,int>& tempGroupData)
{
     std::string groupName = group.getName();

        // Создаем узел для группы
        YAML::Node groupNode;

        // Заполняем узел данными из tempGroupData
        groupNode["alias"] = std::get<0>(tempGroupData);
        groupNode["permissions"] = std::get<1>(tempGroupData);
        groupNode["worlds"] = std::get<2>(tempGroupData);
        groupNode["time"] = std::get<3>(tempGroupData);

        // Устанавливаем данные для группы в общем YAML узле
        groups[groupName] = groupNode;

        // Сохраняем изменения в файл
        ofstream fout("plugins/PuePerms/ranks.yaml");
        fout << groups;
        fout.close();
}

void DefaultProvider::setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data)
{
  groups = YAML::Node(YAML::NodeType::Map);

        // Проходим по каждому элементу в unordered_map
        for (const auto& [groupName, groupData] : data) {
            YAML::Node groupNode;

            // Заполняем узел группы данными из tuple
            groupNode["alias"] = std::get<0>(groupData);
            groupNode["permissions"] = std::get<1>(groupData);
            groupNode["worlds"] = std::get<2>(groupData);
            groupNode["time"] = std::get<3>(groupData);

            // Добавляем узел группы в общий узел groups
            groups[groupName] = groupNode;
        }
   ofstream fout("plugins/PuePerms/ranks.yaml");
   fout << groups;
   fout.close();
}
