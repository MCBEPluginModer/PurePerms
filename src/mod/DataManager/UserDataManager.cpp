#include "UserDataManager.h"
#include <mc/world/actor/player/Player.h>

YAML::Node UserDataManager::getData(Player* player)
{
  //return lugin->getProvider()->getPlayerData(player);
  return YAML::Node();
}

std::optional<int> UserDataManager::getExpDate(Player* player,string levelname)
{
   std::optional<int> expDate;

   if (!levelname.empty()) 
   {
        auto worldData = getWorldData(player, levelname);
        if (worldData.has_value()) 
        {
            expDate = worldData.value()["expTime"].as<int>();
        }
        else 
        {
          auto node = getNode(player, "expTime");
          if (node.has_value()) {
                expDate = node.value().as<int>();
          }
        }
   }
   return expDate;
}

optional<PPGroup> UserDataManager::getGroup(Player* player,string levelname)
{
  std::optional<std::string> groupName;

        if (!levelname.empty()) {
            auto worldData = getWorldData(player, levelname);
            if (worldData.has_value()) {
                groupName = worldData.value()["group"].as<std::string>();
            }
        } else {
            auto node = getNode(player, "group");
            if (node.has_value()) {
                groupName = node.value().as<std::string>();
            }
        }

        if (!groupName.has_value()) {
            return nullopt;
        }

       /* PPGroup* group = plugin->getGroup(groupName.value());
        if (group == nullptr) {
            //plugin->getLogger()->critical("Invalid group name found in " + player->getName() + "'s player data (World: " + (levelName.empty() ? "GLOBAL" : levelName) + ")");
            //plugin->getLogger()->critical("Restoring the group data to 'default'");
           // PPGroup* defaultGroup = plugin->getDefaultGroup(levelName);
            //setGroup(player, defaultGroup, levelName);
            //return defaultGroup;
        }

        return group;*/
       return nullopt;
}

optional<YAML::Node> UserDataManager::getNode(Player* player,string node)
{
  // Получаем данные пользователя (например, из файла или базы данных)
        YAML::Node userData = getData(player);
        
        // Проверяем наличие узла
        if (userData[node]) 
        {
            return userData[node];
        }
        
        // Если узел не найден, возвращаем std::nullopt
        return std::nullopt;
}

vector<string> UserDataManager::getUserPermissions(Player* player,string levelname)
{
  YAML::Node permissions;
        if (!levelname.empty()) {
            auto worldData = getWorldData(player, levelname);
            if (worldData && worldData->IsMap() && worldData->operator[]("permissions")) {
                permissions = (*worldData)["permissions"];
            }
        } else {
            auto node = getNode(player, "permissions");
            if (node && node->IsSequence()) {
                permissions = *node;
            }
        }

        // Проверка на корректность данных permissions
        if (!permissions || !permissions.IsSequence()) {
           // std::cerr << "Invalid 'permissions' node given to getUserPermissions()" << std::endl;
            return {}; // Возвращаем пустой вектор, если данные не корректны
        }

        // Преобразуем YAML::Node в std::vector<std::string>
        std::vector<std::string> result;
        for (const auto& perm : permissions) {
            result.push_back(perm.as<std::string>());
        }

        return result;
}

optional<YAML::Node> UserDataManager::getWorldData(Player* player,string levelname)
{
  if (levelname.empty()) {
            levelname = "Overworld";
        }

        YAML::Node userData = getData(player);
        if (!userData["worlds"] || !userData["worlds"][levelname]) {
            // Если данных для данного мира нет, возвращаем стандартные значения
            YAML::Node defaultData;
           // defaultData["group"] = this->plugin->getDefaultGroup(levelname)->getName();
            defaultData["group"] ="player";
            defaultData["permissions"] = YAML::Node(YAML::NodeType::Sequence);  // Пустой список разрешений
            defaultData["expTime"] = -1;
            return defaultData;
        }

        return userData["worlds"][levelname];
}

void UserDataManager::removeNode(Player* player,string node)
{
  YAML::Node tempUserData = getData(player);

  if (tempUserData[node]) 
  {  // Проверка на наличие узла
      tempUserData.remove(node);  // Удаление узла
      tuple<string,vector<string>,YAML::Node,int> data1 = std::make_tuple<string,vector<string>,YAML::Node,int>(tempUserData["group"].as<string>(),tempUserData["permissions"].as<vector<string>>(),tempUserData["worlds"].as<YAML::Node>(),tempUserData["time"].as<int>());
      setData(player, data1);
  }
}

void UserDataManager::setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data)
{
     //plugin->getProvider()->setPlayerData($player, $data);
}

void UserDataManager::setGroup(Player* player,PPGroup group,string levelname,int time = -1)
{
    if (levelName.empty()) 
    {
            setNode(player, "group", group.getName());
            setNode(player, "expTime", time);
    } 
    else 
    {
            auto worldData = getWorldData(player, levelName);
            if (worldData) 
            {
                (*worldData)["group"] = group.getName();
                (*worldData)["expTime"] = time;
                setWorldData(player, levelName, *worldData);
            }
        }

        //PPRankChangedEvent ev(player, group);
        //ev.call();
}

void UserDataManager::setNode(Player* player,string node,std::variant<bool,int,float,double,string> value)
{
        YAML::Node userData = getData(player);

        // Преобразуем значение в YAML::Node в зависимости от типа
        if (std::holds_alternative<bool>(value)) {
            userData[node] = std::get<bool>(value);
        } else if (std::holds_alternative<int>(value)) {
            userData[node] = std::get<int>(value);
        } else if (std::holds_alternative<float>(value)) {
            userData[node] = std::get<float>(value);
        } else if (std::holds_alternative<double>(value)) {
            userData[node] = std::get<double>(value);
        } else if (std::holds_alternative<std::string>(value)) {
            userData[node] = std::get<std::string>(value);
        }
        
      tuple<string,vector<string>,YAML::Node,int> data1 = std::make_tuple<string,vector<string>,YAML::Node,int>(userData["group"].as<string>(),userData["permissions"].as<vector<string>>(),userData["worlds"].as<YAML::Node>(),userData["time"].as<int>());
      setData(player, data1);
}

void UserDataManager::setPermission(Player* player,string permission,string levelName)
{
    if (levelName.empty()) {
            YAML::Node tempUserData = getData(player);
            tempUserData["permissions"].push_back(permission);
            tuple<string,vector<string>,YAML::Node,int> data1 = std::make_tuple<string,vector<string>,YAML::Node,int>(tempUserData["group"].as<string>(),tempUserData["permissions"].as<vector<string>>(),tempUserData["worlds"].as<YAML::Node>(),tempUserData["time"].as<int>());
            setData(player, data1);
        } else {
            YAML::Node worldData = getWorldData(player, levelName);
            worldData["permissions"].push_back(permission);
            setWorldData(player, levelName, worldData);
        }

        //plugin->updatePermissions(player);
}

void UserDataManager::setWorldData(Player* player,string levelName,tuple<bool,vector<string>> worldData)
{
     YAML::Node tempUserData = getData(player);

        if (!tempUserData["worlds"] || !tempUserData["worlds"][levelName]) {
            // Если данные для мира не существуют, создаем их
            tempUserData["worlds"][levelName] = YAML::Node();
           // tempUserData["worlds"][levelName]["group"] = plugin->getDefaultGroup();
            tempUserData["worlds"][levelName]["group"] = "player";
            tempUserData["worlds"][levelName]["permissions"] = YAML::Node(YAML::NodeType::Sequence);
            tempUserData["worlds"][levelName]["expTime"] = -1;

            tuple<string,vector<string>,YAML::Node,int> data1 = std::make_tuple<string,vector<string>,YAML::Node,int>(tempUserData["group"].as<string>(),tempUserData["permissions"].as<vector<string>>(),tempUserData["worlds"].as<YAML::Node>(),tempUserData["time"].as<int>());
            setData(player, data1);
        }

        // Устанавливаем переданные данные для мира
        tempUserData["worlds"][levelName]["group"] = std::get<0>(worldData);
        tempUserData["worlds"][levelName]["permissions"] = YAML::Node(YAML::NodeType::Sequence);
        for (const auto& permission : std::get<1>(worldData)) {
            tempUserData["worlds"][levelName]["permissions"].push_back(permission);
        }
        tempUserData["worlds"][levelName]["expTime"] = std::get<2>(worldData);

        tuple<string,vector<string>,YAML::Node,int> data1 = std::make_tuple<string,vector<string>,YAML::Node,int>(tempUserData["group"].as<string>(),tempUserData["permissions"].as<vector<string>>(),tempUserData["worlds"].as<YAML::Node>(),tempUserData["time"].as<int>());
        setData(player, data1);
}

void UserDataManager::unsetPermission(Player* player,string permission,string levelName)
{
    if (levelName.empty()) 
    {
            YAML::Node tempUserData = getData(player);
            if (!tempUserData["permissions"].IsSequence()) return;

            auto& permissions = tempUserData["permissions"];
            auto it = std::remove(permissions.begin(), permissions.end(), permission);
            if (it != permissions.end()) {
                permissions.erase(it, permissions.end());
                tuple<string,vector<string>,YAML::Node,int> data1 = std::make_tuple<string,vector<string>,YAML::Node,int>(tempUserData["group"].as<string>(),tempUserData["permissions"].as<vector<string>>(),tempUserData["worlds"].as<YAML::Node>(),tempUserData["time"].as<int>());
                setData(player, data1);
            }
        } else {
            auto worldDataOpt = getWorldData(player, levelName);
            if (!worldDataOpt.has_value() || !worldDataOpt->IsMap()) return;

            auto& worldData = worldDataOpt.value();
            if (!worldData["permissions"].IsSequence()) return;

            auto& permissions = worldData["permissions"];
            auto it = std::remove(permissions.begin(), permissions.end(), permission);
            if (it != permissions.end()) {
                permissions.erase(it, permissions.end());
                setWorldData(player, levelName, worldData);
            }
        }

        //plugin->updatePermissions(player);
}
