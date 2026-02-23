#include "pc/test/PrimordialCoreTest.h"
#include "pc/callback/CallbackManager.hpp"
#include "pc/world/Game.h"

#include "ll/api/mod/RegisterHelper.h"

namespace pc {

PrimordialCoreTest& PrimordialCoreTest::getInstance() {
    static PrimordialCoreTest instance;
    return instance;
}

bool PrimordialCoreTest::load() {
    getLogger().debug("Loading...");
    // Code for loading the mod goes here.

    CallbackManager& mgr = CallbackManager::getInstance();

    mgr.addCallback("LevelInitialize", [](bool isClientSide, uint64 levelId, std::string levelName) {
        if (isClientSide) getLogger().info("OnClientLevelInitialize: {}, {}", levelId, levelName);
        else getLogger().info("OnServerLevelInitialize: {}, {}", levelId, levelName);
    });

    return true;
}

bool PrimordialCoreTest::enable() {
    getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool PrimordialCoreTest::disable() {
    getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

bool PrimordialCoreTest::unload() {
    getLogger().debug("Unloading...");
    // Code for unloading the mod goes here.
    return true;
}

} // namespace pc

LL_REGISTER_MOD(pc::PrimordialCoreTest, pc::PrimordialCoreTest::getInstance());
