#include "JsonProvider.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#pragma comment(lib, "bedrock_server_api.lib")

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
    rapidjson::Document userConfig;
    userConfig.SetObject();

    if (onUpdate) {
        if (!fileExists(filePath)) {
            userConfig.AddMember("userName", rapidjson::Value(userName.c_str(), userConfig.GetAllocator()), userConfig.GetAllocator());
            userConfig.AddMember("group", "player", userConfig.GetAllocator());
            userConfig.AddMember("permissions", rapidjson::kArrayType, userConfig.GetAllocator());
            userConfig.AddMember("worlds", rapidjson::kArrayType, userConfig.GetAllocator());
            userConfig.AddMember("time", -1, userConfig.GetAllocator());

            saveConfig(filePath, userConfig);
        } else {
            userConfig = loadConfig(filePath);
        }
    } else {
        if (fileExists(filePath)) {
            userConfig = loadConfig(filePath);
        } else {
            userConfig.AddMember("userName", rapidjson::Value(userName.c_str(), userConfig.GetAllocator()), userConfig.GetAllocator());
            userConfig.AddMember("group", "player", userConfig.GetAllocator());
            userConfig.AddMember("permissions", rapidjson::kArrayType, userConfig.GetAllocator());
            userConfig.AddMember("worlds", rapidjson::kArrayType, userConfig.GetAllocator());
            userConfig.AddMember("time", -1, userConfig.GetAllocator());
        }
    }

    return userConfig;

std::tuple<std::string, std::vector<std::string>, YAML::Node, int> JsonProvider::getPlayerData(Player* player) 
{
      std::string userName = player->getRealName();
    std::string lowerUserName = toLowerCase(userName);

    std::string jsonFilePath = "plugins/PurePerms/players/" + lowerUserName + ".json";
    YAML::Node userConfig;
    std::vector<std::string> permissions;
    int time = -1;

    if (std::filesystem::exists(jsonFilePath)) {
        rapidjson::Document jsonData = loadConfig(jsonFilePath);

        if (jsonData.HasMember("permissions") && jsonData["permissions"].IsArray()) {
            for (const auto& perm : jsonData["permissions"].GetArray()) {
                permissions.push_back(perm.GetString());
            }
        }
        if (jsonData.HasMember("time")) {
            time = jsonData["time"].GetInt();
        }

        userConfig["userName"] = jsonData["userName"].GetString();
        userConfig["group"] = jsonData["group"].GetString();
        userConfig["permissions"] = permissions;
        userConfig["time"] = time;
    } else {
        userConfig["userName"] = userName;
        userConfig["group"] = "player";
        permissions = {};
        time = -1;
    }

    return std::make_tuple(userConfig["group"].as<std::string>(), permissions, userConfig, time);
}

optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> JsonProvider::getUsers()
{
  std::unordered_map<std::string, std::tuple<std::string, std::vector<std::string>, YAML::Node, int>> users;

    std::string playersDir = "plugins/PurePerms/players/";

    if (!std::filesystem::exists(playersDir)) {
        return std::nullopt;
    }

    for (const auto& entry : std::filesystem::directory_iterator(playersDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::string fileName = entry.path().filename().stem().string();

            rapidjson::Document jsonData = loadConfig(entry.path().string());

            std::string group = jsonData["group"].GetString();
            std::vector<std::string> permissions;
            for (const auto& perm : jsonData["permissions"].GetArray()) {
                permissions.push_back(perm.GetString());
            }
            int time = jsonData["time"].GetInt();

            YAML::Node yamlData;
            for (auto& m : jsonData.GetObject()) {
                yamlData[m.name.GetString()] = m.value.GetString();
            }

            users[fileName] = std::make_tuple(group, permissions, yamlData, time);
        }
    }

    if (!users.empty()) {
        return users;
    } else {
        return std::nullopt;
    }
}

void JsonProvider::setGroupData(PPGroup& group, tuple<string,vector<string>,YAML::Node,int>& tempGroupData)
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

void JsonProvider::setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data)
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

rapidjson::Value JsonProvider::convertYamlToJson(const YAML::Node& yamlNode, rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value jsonValue;

    if (yamlNode.IsScalar()) {
        jsonValue.SetString(yamlNode.Scalar().c_str(), allocator);
    } else if (yamlNode.IsSequence()) {
        jsonValue.SetArray();
        for (const auto& element : yamlNode) {
            jsonValue.PushBack(convertYamlToJson(element, allocator), allocator);
        }
    } else if (yamlNode.IsMap()) {
        jsonValue.SetObject();
        for (const auto& element : yamlNode) {
            rapidjson::Value key(element.first.as<std::string>().c_str(), allocator);
            jsonValue.AddMember(key, convertYamlToJson(element.second, allocator), allocator);
        }
    }

    return jsonValue;
}

void JsonProvider::saveConfig(const std::string& filePath, const rapidjson::Document& config) {
    std::ofstream ofs(filePath);
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    config.Accept(writer);
}

// Method to load JSON configuration
rapidjson::Document JsonProvider::loadConfig(const std::string& filePath) {
    std::ifstream ifs(filePath);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document config;
    config.ParseStream(isw);
    return config;
}

void JsonProvider::setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data)
{
  std::string userName = player->getRealName();
    std::string filePath = userDataFolder + toLowerCase(userName) + ".json";

    rapidjson::Document userConfig;
    userConfig.SetObject();
    rapidjson::Document::AllocatorType& allocator = userConfig.GetAllocator();

    userConfig.AddMember("userName", rapidjson::Value(userName.c_str(), allocator), allocator);
    userConfig.AddMember("group", rapidjson::Value(std::get<0>(data).c_str(), allocator), allocator);

    rapidjson::Value permissions(rapidjson::kArrayType);
    for (const auto& perm : std::get<1>(data)) {
        permissions.PushBack(rapidjson::Value(perm.c_str(), allocator), allocator);
    }
    userConfig.AddMember("permissions", permissions, allocator);

    userConfig.AddMember("worlds", convertYamlToJson(std::get<2>(data), allocator), allocator);
    userConfig.AddMember("time", std::get<3>(data), allocator);

    saveConfig(filePath, userConfig);
}
