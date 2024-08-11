#include "ProviderInterface.h"
#include <yaml-cpp/yaml.h>
#include <ll/api/Logger.h>
#include <mc/world/actor/player/Player.h>
#include "../MyMod.h"
#include "../PPGroup.h"
#include <unordered_map>
#include <optional>
#include <SQLiteCpp/SQLiteCpp.h>

class JsonProvider : public ProviderInterface
{
   mcpm::PurePerms* plugin;
   SQLite::Database* db;
   YAML::Node groups;
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
   JsonProvider(mcpm::PurePerms* _plugin) : db("plugins/PurePerms/pureperms.db")  {
     plugin = _plugin;
     db.exec(""); 
     loadGroupsData();
   }
   void loadGroupData() {}
   YAML::Node getGroupData(PPGroup group) {}
   YAML::Node getGroupsConfig() {return groups;}
   YAML::Node getGroupsData() {return groups;}
   rapidjson::Document getPlayerConfig(Player* player, bool onUpdate = false) {}
   tuple<string,vector<string>,YAML::Node,int> getPlayerData(Player* player) {}
   optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> getUsers() {}
   void setGroupData(PPGroup& group,tuple<string,vector<string>,YAML::Node,int>& data) {}
   void setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data) {}
   void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data) {}
   void close() {}
};
