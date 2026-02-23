#include "magic_enum/magic_enum.hpp"

#include "mc/world/level/Level.h"
#include "mc/world/level/Tick.h"
#include "mc/world/Minecraft.h"
#include "mc/server/ServerInstance.h"

#include "ll/api/memory/Hook.h"

#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"
#include "pc/world/Game.h"

using namespace pc;

LL_AUTO_TYPE_INSTANCE_HOOK(
    LevelInitializeHook,
    HookPriority::Normal,
    ::Level,
    &::Level::$initialize,
    bool,
    ::std::string const&   levelName,
    ::LevelSettings const& levelSettings,
    ::Experiments const&   experiments,
    ::std::string const*   levelId,
    ::std::optional<::std::reference_wrapper<
        ::std::unordered_map<::std::string, ::std::unique_ptr<::BiomeJsonDocumentGlue::ResolvedBiomeData>>>>
        biomeIdToResolvedData
) {
    CallbackManager::getInstance().invokeCallback("LevelInitialize", isClientSide(), uint64(levelId), levelName);
    return origin(levelName, levelSettings, experiments, levelId, biomeIdToResolvedData);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    LevelTickHook,
    HookPriority::Normal,
    ::Level,
    &::Level::$tick,
    void
) {
    {
        Game::EventScope scope;
        CallbackManager::getInstance().invokeCallback("LevelTick", isClientSide(), getCurrentTick().tickID);
        if (isClientSide()) {
#if defined(LL_PLAT_C)
            CallbackManager::getInstance().invokeCallback("tick");
#endif
        } else {
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

LL_AUTO_TYPE_INSTANCE_HOOK(
    MinecraftStartLeaveGameHook,
    HookPriority::Normal,
    ::Minecraft,
    &::Minecraft::startLeaveGame,
    void,
    bool stopNetwork
) {
    CallbackManager::getInstance().invokeCallback("StartLeaveGame", getLevel()->isClientSide(), stopNetwork);
    return origin(stopNetwork);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ServerInstanceOnLevelCorruptHook,
    HookPriority::Normal,
    ::ServerInstance,
    &::ServerInstance::$onLevelCorrupt,
    void
) {
    CallbackManager::getInstance().invokeCallback("LevelCorrupt", false);
    return origin();
}
