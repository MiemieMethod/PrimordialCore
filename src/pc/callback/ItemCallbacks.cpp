#include "ll/api/memory/Hook.h"

#include "mc/world/gamemode/InteractionResult.h"
#include "mc/world/item/Item.h"
#include "mc/world/item/ItemUsedOnEventContext.h"

LL_AUTO_TYPE_INSTANCE_HOOK(
    ItemOnUseHook,
    HookPriority::Normal,
    Item,
    &Item::useOn,
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
    return origin(item, entity, x, y, z, face, clickPos, itemUsedOnEventContext);
}
