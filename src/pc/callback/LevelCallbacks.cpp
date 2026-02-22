#include "mc/world/level/Level.h"
#include "mc/world/level/Tick.h"

#include "ll/api/memory/Hook.h"

#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"
#include "pc/world/Game.h"

using namespace pc;

LL_AUTO_TYPE_INSTANCE_HOOK(
    LevelTickHook,
    HookPriority::Normal,
    ::Level,
    &::Level::$tick,
    void) {
    {
        Game::EventScope scope;
        if (isClientSide()) {
            CallbackManager::getInstance().invokeCallback("OnClientLevelTick", getCurrentTick().tickID);
#if defined(LL_PLAT_C)
            CallbackManager::getInstance().invokeCallback("tick");
#endif
        } else {
            CallbackManager::getInstance().invokeCallback("OnServerLevelTick", getCurrentTick().tickID);
#if defined(LL_PLAT_S)
            CallbackManager::getInstance().invokeCallback("tick");
#endif
        }
        if (scope.isPrevented()) {
            PrimordialCore::getLogger().debug("Level::tick was prevented by a callback!");
            return;
        }
    }
    return origin();
}
