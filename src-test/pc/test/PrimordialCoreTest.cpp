#include "pc/test/PrimordialCoreTest.h"
#include "pc/callback/CallbackManager.hpp"
#include "pc/world/Game.h"

#include "ll/api/mod/RegisterHelper.h"

namespace pc {

void Hooked_OriginalFunction(CallbackManager& mgr, int damage) {
    Game::EventScope scope;

    mgr.invokeCallback("OnTakeDamage", damage);

    if (scope.isPrevented()) {
        PrimordialCoreTest::getLogger().warn("Original default functionality was prevented by a callback!");
        return; // 直接返回，从而阻止原函数的默认行为
    }

    PrimordialCoreTest::getLogger().info("Executing default original functionality. Damage: {}", damage);
}

PrimordialCoreTest& PrimordialCoreTest::getInstance() {
    static PrimordialCoreTest instance;
    return instance;
}

bool PrimordialCoreTest::load() {
    getLogger().debug("Loading...");
    // Code for loading the mod goes here.

    CallbackManager& mgr = CallbackManager::getInstance();

    // 添加不同签名的事件
    mgr.addCallback(
        "OnLogin",
        [](std::string user, int age) { getLogger().info("[User] {} logged in. Age: {}", user, age); },
        10
    );
    
    mgr.addCallback("OnSystem", [](int code) { getLogger().info("Code: {}", code); });
    
    // 高优先级测试
    mgr.addCallback(
        "OnLogin",
        [](std::string user, int) { getLogger().info("[Admin] Priority check for {}", user); },
        100
    );

    // 调用
    getLogger().info("--- Triggering Login ---");
    // 注意：必须显式构造std::string，否则推导为const char*导致类型不匹配
    mgr.invokeCallback("OnLogin", std::string("Alice"), 25);

    getLogger().info("--- Triggering System ---");
    mgr.invokeCallback("OnSystem", 404);

    // 回调 A：不阻止默认行为
    mgr.addCallback(
        "OnTakeDamage",
        [](int damage) { getLogger().info("Callback A saw damage: {}", damage); },
        10
    );

    // 回调 B：满足特定条件时阻止默认行为
    mgr.addCallback(
        "OnTakeDamage",
        [](int damage) {
            if (damage > 100) {
                getLogger().warn("Callback B: Damage too high! Preventing default.");
                Game::getInstance().preventDefault(); // 直接调用，无需返回特定类型
            }
        },
        20
    );

    getLogger().info("=== 测试1：不会被阻止（damage=50） ===");
    Hooked_OriginalFunction(mgr, 50);

    getLogger().info("=== 测试2：会被回调 B 阻止（damage=150） ===");
    Hooked_OriginalFunction(mgr, 150);

    // 测试嵌套调用
    getLogger().info("=== 测试3：嵌套调用测试 ===");
    mgr.addCallback(
        "OnTakeDamage",
        [&mgr](int damage) {
            getLogger().info("Callback C: Triggering nested event");
            Game::EventScope scope;
            mgr.invokeCallback("OnNestedEvent", damage);
        },
        30
    );

    mgr.addCallback("OnNestedEvent", [](int damage) {
        getLogger().info("Nested callback: Damage = {}", damage);
        // 这里调用 preventDefault 不会影响外层 OnTakeDamage 的状态
        Game::getInstance().preventDefault();
    });

    {
        Game::EventScope scope;
        mgr.invokeCallback("OnTakeDamage", 200);
        getLogger().info("Outer scope prevented: {}", scope.isPrevented() ? "Yes" : "No");
    }

    {
        Game::EventScope scope;
        mgr.invokeCallback("OnTakeDamage", 20);
        getLogger().info("Outer scope prevented: {}", scope.isPrevented() ? "Yes" : "No");
    }

    //mgr.addCallback("OnServerLevelTick", [](uint64 tick) { getLogger().info("OnServerLevelTick: {}", tick); });
    //mgr.addCallback("OnClientLevelTick", [](uint64 tick) { getLogger().info("OnClientLevelTick: {}", tick); });
    //mgr.addCallback("tick", []() { getLogger().info("tick"); });

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
