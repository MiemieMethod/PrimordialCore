#include "ll/api/mod/RegisterHelper.h"

#include "pc/mod/PrimordialCore.h"

namespace pc {

PrimordialCore& PrimordialCore::getInstance() {
    static PrimordialCore instance;
    return instance;
}

bool PrimordialCore::load() {
    getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool PrimordialCore::enable() {
    getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool PrimordialCore::disable() {
    getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

bool PrimordialCore::unload() {
    getLogger().debug("Unloading...");
    // Code for unloading the mod goes here.
    return true;
}

} // namespace pc

LL_REGISTER_MOD(pc::PrimordialCore, pc::PrimordialCore::getInstance());
