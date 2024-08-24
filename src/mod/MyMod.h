#pragma once

#include "ll/api/mod/NativeMod.h"
#include "PPMessages.h"
#include "provider/ProviderInterface.h"
#include <unordered_map>

class UserDataManager;
class PPGroup;

namespace mcpm {

class PurePerms {

public:
    static PurePerms& getInstance();

    PurePerms(ll::mod::NativeMod& self) : mSelf(self) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    // bool unload();

   
    const std::string MAIN_PREFIX = "\x5b\x50\x75\x72\x65\x50\x65\x72\x6d\x73\x3a\x36\x34\x46\x46\x30\x30\x5d";
    const std::string CORE_PERM = "\x70\x70\x65\x72\x6d\x73\x2e\x63\x6f\x6d\x6d\x61\x6e\x64\x2e\x70\x70\x69\x6e\x66\x6f";
    const int NOT_FOUND = -1;  // В C++ нет null для целочисленных констант, поэтому используем -1
    const int INVALID_NAME = -1;
    const int ALREADY_EXISTS = 0;
    const int SUCCESS = 1;
    bool isGroupsLoaded = false;

    // Указатели на другие классы
    PPMessages* messages = nullptr;
    ProviderInterface* provider = nullptr;
    UserDataManager* userDataMgr = nullptr;
    std::vector<std::string> attachments;
    std::vector<std::string> groups;
    std::unordered_map<std::string, std::string> bdsDefaultPerms;
    void setProvider(bool onEnab = true);
    bool addGroup(string groupName);
    int date2Int(string date);
    vector<string> getAttachment(Player* player);
    optional<variant<bool,int,double,string,YAML::Node>> getConfigValue(string key);
    optional<PPGroup> getDefaultGroup(string levelname = "");
    optional<PPGroup> getGroup(string groupName);
    vector<PPGroup> getGroups();
    template<typename... Args>
    std::string getMessage(const std::string& node, Args&&... vars);
    vector<Player*> getOnlinePlayersInGroup(PPGroup group);
    vector<string> getPermissions(Player* player,string levelname);
    Player* getPlayer(string nickname);
    unordered_map<string,string> getBDSPerms();
    string getLLVersion() {return "0.13.5";}
    ProviderInterface* getProvider();
    UserDataManager* getUserDataMgr() {return userDataMgr;}
    string getValidUUID(Player* player);
    int isValidGroupName(string groupName);
    bool isValidProvider();
    void registerPlayer(Player* player);
    void registerPlayers();
    bool removeGroup(string groupName);
    void setDefaultGroup(PPGroup* group,string levelname);
    void setGroup(Player* player,PPGroup group,string levelname = "",int time = -1);
    void sortGroupData();
    void updateGroups();
    void updatePermissions(Player* player,string levelname = "");
    void updatePlayersInGroup(PPGroup* group);
    void unregisterPlayer(Player* player);
    void unregisterPlayers();
private:
    ll::mod::NativeMod& mSelf;
};

} // namespace my_mod
