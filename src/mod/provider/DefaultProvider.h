#include "ProviderInterface.h"
#include <yaml-cpp/yaml.h>
#include <ll/api/Logger.h>
#include <mc/world/actor/player/Player.h>
#include "../MyMod.h"
#include "../PPGroup.h"
#include <unordered_map>
#include <optional>

class DefaultProvider : public ProviderInterface
{
   mcpm::PurePerms* plugin;
   YAML::Node groups,players;
   inline std::string toLowerCase(const std::string& str) const 
   {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
public:
   DefaultProvider(mcpm::PurePerms* _plugin);
   YAML::Node getGroupData(const PPGroup& group);
   YAML::Node getGroupsConfig() {return groups;}
   YAML::Node getGroupsData() {return groups;}
   tuple<string,vector<string>,YAML::Node,int> getPlayerData(const Player* player);
   optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> getUsers();
   void setGroupData(PPGroup& group,tuple<string,vector<string>,vector<string>,YAML::Node,int>& data);
   void setGroupsData(unordered_map<string,PPGroup> data);
   void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data);
   void close() {}
};
