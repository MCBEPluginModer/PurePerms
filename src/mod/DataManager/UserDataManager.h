#pragma once
#include "../MyMod.h"
#include "../EventManager/PPRankExpiredEvent.h"
#include <optional>
#include <vector>
using namespace std;

class UserDataManager
{
   mcpm::PurePerms* plugin;
public:
    UserDataManager(mcpm::PurePerms* _plugin) : plugin(_plugin) {}
    YAML::Node getData(Player* player);
    std::optional<int> getExpDate(Player* player,string levelname = "");
    optional<PPGroup> getGroup(Player* player,string levelname = "");
    optional<YAML::Node> getNode(Player* player,string node);
    vector<string> getUserPermissions(Player* player,string levelname);
    optional<YAML::Node> getWorldData(Player* player,string levelname);
    void removeNode(Player* player,string node);
    void setData(Player* player,tuple<string,vector<string>,YAML::Node,int> data) {
       //plugin->getProvider()->setPlayerData(player, data);
    }
    void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data);
    void setGroup(Player* player,PPGroup group,string levelname,int time = -1);
    void setNode(Player* player,string node,std::variant<bool,int,float,double,string> value);
    void setPermission(Player* player,string permission,string levelName = "");
    void setWorldData(Player* player,string levelName,tuple<bool,vector<string>> worldData);
    void unsetPermission(Player* player,string permission,string levelName = "");
};
