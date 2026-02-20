#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"
#include "pc/world/Game.h"

#include "ll/api/mod/RegisterHelper.h"

namespace primordial_core {

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

} // namespace primordial_core

LL_REGISTER_MOD(primordial_core::PrimordialCore, primordial_core::PrimordialCore::getInstance());
