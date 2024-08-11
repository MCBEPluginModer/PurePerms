#include "ProviderInterface.h"
#include <yaml-cpp/yaml.h>
#include <ll/api/Logger.h>
#include <mc/world/actor/player/Player.h>
#include "../MyMod.h"
#include "../PPGroup.h"
#include <unordered_map>
#include <optional>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

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
    void saveConfig(const std::string& filePath, const rapidjson::Document& config);
    rapidjson::Document loadConfig(const std::string& filePath);
    rapidjson::Value convertYamlToJson(const YAML::Node& yamlNode, rapidjson::Document::AllocatorType& allocator);
public:
   JsonProvider(mcpm::PurePerms* _plugin);
   YAML::Node getGroupData(PPGroup group);
   YAML::Node getGroupsConfig() {return groups;}
   YAML::Node getGroupsData() {return groups;}
   rapidjson::Document getPlayerConfig(Player* player, bool onUpdate = false);
   tuple<string,vector<string>,YAML::Node,int> getPlayerData(Player* player);
   optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> getUsers();
   void setGroupData(PPGroup& group,tuple<string,vector<string>,YAML::Node,int>& data);
   void setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data);
   void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data);
   void close() {}
};
