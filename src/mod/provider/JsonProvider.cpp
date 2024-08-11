#include "JsonProvider.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

JsonProvider::JsonProvider(mcpm::PurePerms* _plugin)
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
    userDataFolder = "plugins/PurePerms/players";
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
    userDataFolder = "plugins/PurePerms/players";
     // Проверяем, существует ли папка с данными пользователей, если нет — создаем
    if (!fs::exists(userDataFolder)) {
      fs::create_directories(userDataFolder);
    }
  }
}

YAML::Node JsonProvider::getGroupData(PPGroup group)
{
  std::string groupName = group.getName();
  YAML::Node groupsData = getGroupsData();

        // Проверка наличия данных для группы
  if (!groupsData[groupName] || !groupsData[groupName].IsMap()) {
      return YAML::Node(YAML::NodeType::Undefined); // Возвращаем неопределенный узел
  }

  return groupsData[groupName];
}

Json::Value JsonProvider::getPlayerConfig(Player* player, bool onUpdate)
{
    std::string userName = player->getRealName();
        std::string filePath = userDataFolder + toLowerCase(userName) + ".json";
        Json::Value userConfig;

        if (onUpdate) {
            if (!fileExists(filePath)) {
                userConfig["userName"] = userName;
                userConfig["group"] = "player";
                userConfig["permissions"] = Json::Value(Json::arrayValue);
                userConfig["worlds"] = Json::Value(Json::arrayValue);
                userConfig["time"] = -1;

                saveConfig(filePath, userConfig);
            } else {
                userConfig = loadConfig(filePath);
            }
        } else {
            if (fileExists(filePath)) {
                userConfig = loadConfig(filePath);
            } else {
                userConfig["userName"] = userName;
                userConfig["group"] = "player";
                userConfig["permissions"] = Json::Value(Json::arrayValue);
                userConfig["worlds"] = Json::Value(Json::arrayValue);
                userConfig["time"] = -1;
            }
        }

        return userConfig;
}

std::tuple<std::string, std::vector<std::string>, YAML::Node, int> JsonProvider::getPlayerData(Player* player) 
{
        std::string userName = player->getRealName();
        std::string lowerUserName = toLowerCase(userName);

        // Путь к JSON файлу с данными игрока
        std::string jsonFilePath = "plugins/PurePerms/players/" + lowerUserName + ".json";
        YAML::Node userConfig;
        std::vector<std::string> permissions;
        int time = -1;

        // Проверяем, существует ли JSON файл конфигурации
        if (std::filesystem::exists(jsonFilePath)) {
            // Считываем данные из JSON файла
            Json::Value jsonData;
            std::ifstream configFile(jsonFilePath, std::ifstream::binary);
            configFile >> jsonData;

            // Преобразуем JSON данные в YAML::Node
            if (jsonData.isMember("permissions")) {
                for (const auto& perm : jsonData["permissions"]) {
                    permissions.push_back(perm.asString());
                }
            }
            if (jsonData.isMember("time")) {
                time = jsonData["time"].asInt();
            }

            userConfig["userName"] = jsonData["userName"].asString();
            userConfig["group"] = jsonData["group"].asString();
            userConfig["permissions"] = permissions;
            userConfig["time"] = time;
        } else {
            // Если файл не существует, возвращаем данные по умолчанию
            userConfig["userName"] = userName;
            userConfig["group"] = "player";
            permissions = {};  // Пустой список разрешений
            time = -1;
        }

        // Возвращаем данные в виде кортежа
        return std::make_tuple(userConfig["group"].as<std::string>(), permissions, userConfig, time);
    }
