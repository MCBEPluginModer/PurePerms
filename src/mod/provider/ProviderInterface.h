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
    virtual unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> getUsers() = 0;
    virtual void setGroupData(const PPGroup& group, const std::unordered_map<std::string, std::string>& tempGroupData) = 0;
    virtual void setGroupsData(const std::vector<std::unordered_map<std::string, std::string>>& tempGroupsData) = 0;
    virtual void setPlayerData(const Player* player, const std::unordered_map<std::string, std::string>& tempPlayerData) = 0;
    virtual void close() = 0;
};
