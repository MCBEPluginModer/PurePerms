#include "ProviderInterface.h"
#include <yaml-cpp/yaml.h>
#include <ll/api/Logger.h>
#include <mc/world/actor/player/Player.h>
#include "../MyMod.h"
#include "../PPGroup.h"
#include <unordered_map>
#include <optional>
#include <json/json.h>

class JsonProvider : public ProviderInterface
{
   mcpm::PurePerms* plugin;
   YAML::Node groups;
   std::string userDataFolder;
   inline std::string toLowerCase(const std::string& str) const 
   {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    bool fileExists(const std::string& filePath) 
    {
        struct stat buffer;
        return (stat(filePath.c_str(), &buffer) == 0);
    }
public:
   JsonProvider(mcpm::PurePerms* _plugin);
   YAML::Node getGroupData(PPGroup group);
   YAML::Node getGroupsConfig() {return groups;}
   YAML::Node getGroupsData() {return groups;}
   Json::Value getPlayerConfig(Player* player, bool onUpdate = false);
   tuple<string,vector<string>,YAML::Node,int> getPlayerData(Player* player);
   optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> getUsers();
   void setGroupData(PPGroup& group,tuple<string,vector<string>,YAML::Node,int>& data);
   void setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data);
   void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data);
   void close() {}
};
