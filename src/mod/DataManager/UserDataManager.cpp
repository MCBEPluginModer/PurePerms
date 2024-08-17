#include "UserDataManager.h"

YAML::Node getData(Player* player)
{
  //return lugin->getProvider()->getPlayerData(player);
}

std::optional<int> getExpDate(Player* player,string levelname)
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
