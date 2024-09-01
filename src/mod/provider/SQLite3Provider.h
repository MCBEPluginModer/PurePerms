#include "ProviderInterface.h"
#include <yaml-cpp/yaml.h>
#include <ll/api/Logger.h>
#include <mc/world/actor/player/Player.h>
#include "../MyMod.h"
#include "../PPGroup.h"
#include <unordered_map>
#include <optional>
#include <SQLiteCpp/SQLiteCpp.h>

class SQLite3Provider : public ProviderInterface
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
   SQLite3Provider(mcpm::PurePerms* _plugin)
    : plugin(_plugin), db(nullptr) { // Initialize db as nullptr first
    try {
        db = new SQLite::Database("plugins/PurePerms/pureperms.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        loadGroupsData();
    } catch (const std::exception& e) {
        // Handle database initialization failure
        std::cerr << "Failed to open database: " << e.what() << std::endl;
        db = nullptr; // Ensure db remains nullptr if initialization fails
    }
}
   void loadGroupsData() {}
   YAML::Node getGroupData(const PPGroup& group) override  {return YAML::Node();}
   YAML::Node getGroupsConfig() {return groups;}
   YAML::Node getGroupsData() override {return groups;}
   rapidjson::Document getPlayerConfig(Player* player, bool onUpdate = false) {return rapidjson::Document();}
   tuple<string,vector<string>,YAML::Node,int> getPlayerData(const Player* player) override  {return {}; }
   optional<unordered_map<string,tuple<string,vector<string>,YAML::Node,int>>> getUsers() {return nullopt;}
   void setGroupData(PPGroup& group,tuple<string,vector<string>,YAML::Node,int>& data) override {return;}
   void setGroupsData(unordered_map<string,tuple<string,vector<string>,YAML::Node,int>> data) override {return;}
   void setPlayerData(Player* player,tuple<string,vector<string>,YAML::Node,int> data) override {return;}
   void close() override {return;}
   ~SQLite3Provider() {delete db;}
};
