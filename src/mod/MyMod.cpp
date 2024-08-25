#include "mod/MyMod.h"

#include <memory>

#include "ll/api/mod/RegisterHelper.h"
#include "PPGroup.h"
#include "DataManager/UserDataManager.h"
#include "ll/api/schedule/Scheduler.h"
#include "ll/api/schedule/Task.h"

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
        ll::service::getLevel().forEachPlayer([](Player& player) 
        {
            auto expTime = instance->getUserDataMgr->getNode(player, "expTime")["expTime"].as<int>();
            if (std::time(nullptr) == expTime) {
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
            }
        })
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
        fout.cose();
        messages = new PPMessages(this);
        userDataMgr = new UserDataManager();
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

void PurePerms::setProvider(bool onEnab)
{
    
}

} // namespace my_mod

LL_REGISTER_MOD(mcpm::PurePerms, mcpm::instance);
