#include "mod/MyMod.h"

#include <memory>

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
    AML::Node newGroup;
    newGroup["alias"] = "";
    newGroup["isDefault"] = false;
    newGroup["inheritance"] = YAML::Node(YAML::NodeType::Sequence); // Empty list
    newGroup["permissions"] = YAML::Node(YAML::NodeType::Sequence); // Empty list
    newGroup["worlds"] = YAML::Node(YAML::NodeType::Sequence); // Empty list

    groupsData[groupName] = newGroup;

    getProvider()->setGroupsData(groupsData);
    updateGroups();

    return SUCCESS;
}

#include <regex>
        
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

} // namespace my_mod

LL_REGISTER_MOD(mcpm::PurePerms, mcpm::instance);
