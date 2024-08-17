#pragma once
#include "../MyMod.h"
#include "../PPGroup.h"
#include "EventManager/PPRankExpiredEvent.h"
#include <optional>
#include <vector>
using namespace std;

class UserDataManager
{
   mcpm::PurePerms* plugin;
public:
    YAML::Node getData(Player* player);
    YAML::Node getExpDate(Player* player,string levelname = "");
    optional<PPGroup> getGroup(Player* player,string levelname = "");
    optional<YAML::Node> getNode(Player* player,string node);
    vector<string> getUserPermissions(Player* player,string node);
    optional<YAML::Node> getWorldData(Player* player,string levelname);
    void removeNode(Player* player,string node);
    void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data);
    void setGroup(Player* player,PPGroup group,string levelname,int time = -1);
    
};
