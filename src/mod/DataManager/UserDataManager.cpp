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

optional<PPGroup> UserDataManager::getGroup(Player* player,string levelname = "")
{
  std::optional<std::string> groupName;

        if (!levelName.empty()) {
            auto worldData = getWorldData(player, levelName);
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
}

