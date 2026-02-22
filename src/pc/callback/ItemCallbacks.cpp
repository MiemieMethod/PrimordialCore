#include "mc/world/gamemode/InteractionResult.h"
#include "mc/world/item/Item.h"
#include "mc/world/item/ItemUsedOnEventContext.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/Tick.h"

#include "ll/api/memory/Hook.h"

#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"

using namespace pc;

LL_AUTO_TYPE_INSTANCE_HOOK(
    ItemOnUseHook,
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
    PrimordialCore::getLogger().info("Current Tick: {}", entity.getLevel().getCurrentServerTick().tickID);
    return origin(item, entity, x, y, z, face, clickPos, itemUsedOnEventContext);
}
