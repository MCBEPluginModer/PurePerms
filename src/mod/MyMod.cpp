#include "mod/MyMod.h"

#include <memory>
#include <regex>
#include <string>
#include <ctime>
#include <exception>

#include "ll/api/mod/RegisterHelper.h"
#include "PPGroup.h"
#include "DataManager/UserDataManager.h"
#include "ll/api/schedule/Scheduler.h"
#include "ll/api/schedule/Task.h"
#include "ll/api/service/Service.h"
#include "ll/api/service/Bedrock.h"
#include "provider/JsonProvider.h"
#include "provider/SQLite3Provider.h"
#include "provider/DefaultProvider.h"

using namespace ll::schedule;

using namespace ll::chrono_literals;

using namespace ll::schedule::task;

SystemTimeScheduler scheduler;

namespace mcpm {

static std::unique_ptr<PurePerms> instance;

PurePerms& PurePerms::getInstance() { return *instance; }

class PPExpDateCheckTask 
{
public:
    PPExpDateCheckTask() {}

    void operator()() 
    {
        ll::service::getLevel()->forEachPlayer([](Player& player) -> bool
        {
            auto expTime = instance->getUserDataMgr()->getNode(&player, "expTime").value().as<int>();
            /*if (std::time(nullptr) == expTime) {
                string WorldName;
                auto mwp = YAML::LoadFile("plugins/PurePerms/config.yml")["enable-multiworld-perms"].as<bool>();
                if (mwp)
                {
                    WorldName = player.getLevel().getDimension().mName;
                }
                else
                    WorldName = "";
                pp::PPRankExpiredEvent event(plugin, player);
                event.call();
            }*/
            return true;
        });
    }
};

bool PurePerms::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool PurePerms::enable() {
    getSelf().getLogger().debug("Enabling...");
    YAML::Node config;
    config["data-provider"] = "json";
    config["disable-op"] = false;
    config["enable-multiworld-perms"] = false;
    vector<string> supadmranks = {"OP"};
    config["superadmin-ranks"] = supadmranks;
    ifstream fin("plugins/PurePerms/config.yml");
    if (!fin.is_open())
    {
        fin.close();
        ofstream fout("plugins/PurePerms/config.yml");
        fout << config;
        fout.close();
        messages = new PPMessages();
        userDataMgr = new UserDataManager(this);
    }
    else
        fin.close();
    setProvider();
    registerPlayers();
    
    return true;
}

bool PurePerms::disable() {
    getSelf().getLogger().debug("Disabling...");
    delete messages;
    delete userDataMgr;
    delete provider;
    return true;
}

void PurePerms::setProvider(bool onEnable)
{
    string providerName = YAML::LoadFile("plugins/PurePerms/config.yml")["data-provider"].as<string>();
    if (providerName == "sqlite3")
    {
        provider = new SQLite3Provider(this);
        if (onEnable)
        {
            getSelf().getLogger().info(getMessage("logger_messages.setProvider_SQLITE3"));
        }
    }
    else if (providerName == "json")
    {
        provider = new JsonProvider(this);
        if (onEnable)
        {
            getSelf().getLogger().info(getMessage("logger_messages.setProvider_JSON"));
        }
    }
    else
    {
        provider = new DefaultProvider(this);
        if (onEnable)
        {
            getSelf().getLogger().info(getMessage("logger_messages.setProvider_NotFound","\'" + providerName + "'"));
        }
    }
    updateGroups();
}

int PurePerms::addGroup(string groupName)
{
    auto groupsData = getProvider()->getGroupsData();
    if (!isValidGroupName(groupName))
    {
        return INVALID_NAME;
    }
    if (groupsData[groupName]) 
    {
        return ALREADY_EXISTS;
    }
    YAML::Node newGroup;
    newGroup["alias"] = "";
    newGroup["isDefault"] = false;
    newGroup["inheritance"] = YAML::Node(YAML::NodeType::Sequence); // Empty list
    newGroup["permissions"] = YAML::Node(YAML::NodeType::Sequence); // Empty list
    newGroup["worlds"] = YAML::Node(YAML::NodeType::Sequence); // Empty list

    groupsData[groupName] = newGroup;
    unordered_map<string,PPGroup> data;
    for (YAML::const_iterator it = groupsData.begin(); it != groupsData.end(); ++it) 
    {
        // Get the group name
        std::string groupName = it->first.as<std::string>();
        YAML::Node group = it->second;
        PPGroup gro(this,groupName);
        auto tp = make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(group["alias"].as<string>(),group["isDefault"].as<bool>(),group["inheritance"].as<vector<string>>(),group["permissions"].as<vector<string>>(),group["worlds"]);
        // Get the values of alias, inheritance, permissions, and worlds
        gro.setData(tp);
        data[groupName] = gro;
    }
    getProvider()->setGroupsData(data);
    updateGroups();

    return SUCCESS;
}

int PurePerms::date2Int(string date)
{
     std::regex pattern(R"((\d+)d(\d+)h(\d+)m)");
     std::smatch result;

    if (std::regex_match(date, result, pattern) && result.size() == 4) {
            int days = std::stoi(result[1]);
            int hours = std::stoi(result[2]);
            int minutes = std::stoi(result[3]);

            // Get the current time as time_t (seconds since epoch)
            std::time_t currentTime = std::time(nullptr);

            // Calculate the future time
            std::time_t futureTime = currentTime + (days * 86400) + (hours * 3600) + (minutes * 60);

            return static_cast<int>(futureTime); // Return as int
    }

        return -1; // Return -1 if the pattern does not match
}

vector<string> PurePerms::getAttachment(Player* player)
{
     std::string uniqueId = getValidUUID(player);

        // Check if the uniqueId exists in the attachments map
        if (attachments.find(uniqueId) == attachments.end()) {
            throw std::runtime_error("Tried to calculate permissions on " + player->getRealName() + " using null attachment");
        }

        return attachments[uniqueId];
}

optional<variant<bool,int,double,string,YAML::Node>> PurePerms::getConfigValue(string key)
{
    variant<bool,int,double,string,YAML::Node> ret;
    YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
    if (key == "data-provider" && !config["data-provider"].IsNull())
    {
        ret = config["data-provider"].as<string>();
        return ret;
    }
    else if (key == "disable-op" && !config["disable-op"].IsNull())
    {
        ret = config["disable-op"].as<bool>();
        return ret;
    }
    else if (key == "default-language" && !config["default-language"].IsNull())
    {
        ret = config["default-language"].as<string>();
        return ret;
    }
    else if (key == "enable-multiworld-perms" && !config["enable-multiworld-perms"].IsNull())
    {
        ret = config["enable-multiworld-perms"].as<bool>();
        return ret;
    }
    else if (key == "superadmin-ranks" && !config["superadmin-ranks"].IsNull())
    {
        ret = config["superadmin-ranks"];
        return ret;
    }
    return nullopt;
}

optional<PPGroup> PurePerms::getDefaultGroup(string levelname)
{
    std::vector<PPGroup> defaultGroups;

        // Find groups that are default for the given world name
        for (PPGroup group : getGroups()) {
            if (group.isDefault(levelname)) {
                defaultGroups.push_back(group);
            }
        }

        // Check how many default groups were found
        if (defaultGroups.size() == 1) {
            return defaultGroups[0];
        } else {
            if (defaultGroups.size() > 1) {
                getSelf().getLogger().info(getMessage("logger_messages.getDefaultGroup_01"));
            } else if (defaultGroups.empty()) {
                getSelf().getLogger().info(getMessage("logger_messages.getDefaultGroup_02"));
            }

            getSelf().getLogger().info(getMessage("logger_messages.getDefaultGroup_03"));

            // Fallback: find a group with no parent groups
            for (PPGroup group : getGroups()) {
                if (group.getParentGroups().empty()) {
                    setDefaultGroup(&group, levelname);
                    return group;
                }
            }
        }

        // No suitable group found
        return std::nullopt;
}

#include "PPGroup.h"

optional<PPGroup> PurePerms::getGroup(string groupName)
{
    for (auto gro : getGroups())
        {
            string n = gro.__toString(),n1 = get<1>(gro.getAlias());
            if (n == groupName || n1 == groupName)
            {
                return gro;
            }
        }
    getSelf().getLogger().info(getMessage("logger_messages.getGroup_01", groupName));
    return std::nullopt;
}

vector<PPGroup> PurePerms::getGroups()
{
    vector<PPGroup> grups;
    YAML::Node grs = YAML::LoadFile("plugins/PurePerms/ranks.yml");
    if (!isGroupsLoaded)
    {
        throw std::runtime_error("No groups loaded, maybe a provider error?");
    }
    for (YAML::const_iterator it = grs.begin(); it != grs.end(); ++it) 
    {
        std::string groupName = it->first.as<std::string>(); // Get the group name (key)
        PPGroup gr;
        gr.name = groupName;
        YAML::Node groupNode = it->second;
         // Iterate over the parameters inside each group
        gr.alias = grs[groupName]["alias"].as<string>();
        gr.IsDefault = grs[groupName]["isDefault"].as<bool>();
        gr.permissions = grs[groupName]["permissions"].as<vector<string>>();
        if (!grs[groupName]["inheritance"].IsNull())
        {
            for (auto gs : grs[groupName]["inheritance"].as<vector<string>>())
            {
               PPGroup par = getGroup(gs).value();
               par.name = gs;
               par.parents.push_back(&par);
            }
        }
        gr.worlds = grs[groupName]["worlds"];
    }
}

vector<Player*> PurePerms::getOnlinePlayersInGroup(PPGroup group)
{
    vector<Player*> users;
     ll::service::getLevel()->forEachPlayer([](Player& player) -> bool
        {
            if (userDataMgr->getGroup(&player,player.getLeve().getLevelId()))
             users.push_back(&player);
            return true;
        });
    return users;
}

} // namespace my_mod

LL_REGISTER_MOD(mcpm::PurePerms, mcpm::instance);
