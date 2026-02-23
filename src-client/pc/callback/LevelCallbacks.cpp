#include "mc/client/game/ClientInstance.h"

#include "ll/api/memory/Hook.h"

#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"

using namespace pc;

LL_AUTO_TYPE_INSTANCE_HOOK(
    ClientInstanceOnLevelExitHook,
    HookPriority::Normal,
    ::ClientInstance,
    &::ClientInstance::$onLevelExit,
    void
) {
    CallbackManager::getInstance().invokeCallback("LevelExit", true);
    return origin();
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ClientInstanceOnLevelCorruptHook,
    HookPriority::Normal,
    ::ClientInstance,
    &::ClientInstance::$onLevelCorrupt,
    void
) {
    CallbackManager::getInstance().invokeCallback("LevelCorrupt", true);
    return origin();
}
