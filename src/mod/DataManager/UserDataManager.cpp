#include "UserDataManager.h"

YAML::Node UserDataManager::getData(Player* player)
{
  //return lugin->getProvider()->getPlayerData(player);
}

std::optional<int> UserDataManager::getExpDate(Player* player,string levelname)
{
   std::optional<YAML::Node> expDate;

   if (!levelname.empty()) {
        auto worldData = getWorldData(player, levelname);
        if (worldData.has_value()) {
            expDate = worldData.value()["expTime"].as<int>();
        }
        } 
        else {
          auto node = getNode(player, "expTime");
          if (node.has_value()) {
                expDate = node.value().as<int>();
          }
        }

   return expDate;
}

optional<PPGroup> UserDataManager::getGroup(Player* player,string levelname)
{
  std::optional<std::string> groupName;

        if (!levelName.empty()) {
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
      setData(player, tempUserData);
  }
}

void UserDataManager::setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data)
{
  
}
