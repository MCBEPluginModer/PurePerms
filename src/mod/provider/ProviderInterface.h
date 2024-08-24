#pragma once
#include <mc/world/actor/player/Player.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
using namespace std;

class PPGroup; // Forward declaration

class ProviderInterface 
{

public:
    virtual ~ProviderInterface() = default;

    virtual YAML::Node getGroupData(const PPGroup& group) = 0;
    virtual YAML::Node getGroupsData() = 0;
    virtual tuple<string,vector<string>,YAML::Node,int> getPlayerData(const Player* player) = 0;
    virtual optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> getUsers() = 0;
    virtual void setGroupData(PPGroup& group,tuple<string,vector<string>,YAML::Node,int>& data) = 0;
    virtual void setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data) = 0;
    virtual void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data) = 0;
    virtual void close() = 0;
};
