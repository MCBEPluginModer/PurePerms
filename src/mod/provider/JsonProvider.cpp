#include "JsonProvider.h"
#include <fstream>
#include <filesystem>
using fs = std::filesystem;

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
