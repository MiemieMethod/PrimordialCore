#include "mc/world/gamemode/InteractionResult.h"
#include "mc/world/item/Item.h"
#include "mc/world/item/ItemUsedOnEventContext.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/Tick.h"
#include "mc/nbt/CompoundTag.h"

#include "ll/api/memory/Hook.h"

#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"

using namespace pc;

LL_AUTO_TYPE_INSTANCE_HOOK(
    ItemOnUsedOnHook,
    HookPriority::Normal,
    ::Item,
    &::Item::useOn,
    ::InteractionResult,
    ::ItemStack&             item,
    ::Actor&                 entity,
    int                      x,
    int                      y,
    int                      z,
    uchar                    face,
    ::Vec3 const&            clickPos,
    ::ItemUsedOnEventContext itemUsedOnEventContext
) {
    CallbackManager::getInstance().invokeCallback(
        "ItemUsedOn",
        entity.getLevel().isClientSide(),
        item.getFullNameHash().mStr,
        item.getAuxValue(),
        std::ref(item.mUserData),
        std::ref(entity.getOrCreateUniqueID()),
        x,
        y,
        z,
        face,
        clickPos.x,
        clickPos.y,
        clickPos.z,
        itemUsedOnEventContext.mIsFirstEvent
    );
    return origin(item, entity, x, y, z, face, clickPos, itemUsedOnEventContext);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ItemOnUsedHook,
    HookPriority::Normal,
    ::Item,
    &::Item::$use,
    ::ItemStack&,
    ::ItemStack& item,
    ::Player&    player
) {
    //PrimordialCore::getLogger().info("ItemOnUsedHook Current Tick: {}", player.getLevel().getCurrentServerTick().tickID);
    return origin(item, player);
}
