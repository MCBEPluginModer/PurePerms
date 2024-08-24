#include "mod/MyMod.h"

#include <memory>

#include "ll/api/mod/RegisterHelper.h"
#include "PPGroup.h"
#include "DataManager/UserDataManager.h"

namespace mcpm {

static std::unique_ptr<PurePerms> instance;

PurePerms& PurePerms::getInstance() { return *instance; }

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
