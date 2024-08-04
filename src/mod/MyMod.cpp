#include "mod/MyMod.h"

#include <memory>

#include "ll/api/mod/RegisterHelper.h"

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
    // Code for enabling the mod goes here.
    return true;
}

bool PurePerms::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(PurePerms::MyMod, PurePerms::instance);
