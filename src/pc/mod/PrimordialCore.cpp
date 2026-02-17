#include "pc/mod/PrimordialCore.h"
#include "pc/callback/CallbackManager.hpp"
#include "pc/world/Game.h"

#include "ll/api/mod/RegisterHelper.h"

namespace primordial_core {

void Hooked_OriginalFunction(CallbackManager& mgr, int damage) {
    Game::EventScope scope;

    mgr.invokeCallback("OnTakeDamage", damage);

    if (scope.isPrevented()) {
        std::cout << "Original default functionality was prevented by a callback!" << std::endl;
        return; // 直接返回，从而阻止原函数的默认行为
    }

    std::cout << "Executing default original functionality. Damage: " << damage << std::endl;
}

PrimordialCore& PrimordialCore::getInstance() {
    static PrimordialCore instance;
    return instance;
}

bool PrimordialCore::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.

    CallbackManager& mgr = CallbackManager::getInstance();

    // 添加不同签名的事件
    mgr.addCallback(
        "OnLogin",
        [](std::string user, int age) { std::cout << "[User] " << user << " logged in. Age: " << age << "\n"; },
        10
    );

    mgr.addCallback("OnSystem", [](int code) { std::cout << "Code: " << code << "\n"; });

    // 高优先级测试
    mgr.addCallback(
        "OnLogin",
        [](std::string user, int) { std::cout << "[Admin] Priority check for " << user << "\n"; },
        100
    );

    // 调用
    std::cout << "--- Triggering Login ---\n";
    // 注意：必须显式构造std::string，否则推导为const char*导致类型不匹配
    mgr.invokeCallback("OnLogin", std::string("Alice"), 25);

    std::cout << "\n--- Triggering System ---\n";
    mgr.invokeCallback("OnSystem", 404);

    // 回调 A：不阻止默认行为
    mgr.addCallback("OnTakeDamage", [](int damage) { std::cout << "Callback A saw damage: " << damage << "\n"; }, 10);

    // 回调 B：满足特定条件时阻止默认行为
    mgr.addCallback(
        "OnTakeDamage",
        [](int damage) {
            if (damage > 100) {
                std::cout << "Callback B: Damage too high! Preventing default.\n";
                Game::getInstance().preventDefault(); // 直接调用，无需返回特定类型
            }
        },
        20
    );

    std::cout << "=== 测试1：不会被阻止（damage=50） ===\n";
    Hooked_OriginalFunction(mgr, 50);

    std::cout << "\n=== 测试2：会被回调 B 阻止（damage=150） ===\n";
    Hooked_OriginalFunction(mgr, 150);

    // 测试嵌套调用
    std::cout << "\n=== 测试3：嵌套调用测试 ===\n";
    mgr.addCallback(
        "OnTakeDamage",
        [&mgr](int damage) {
            std::cout << "Callback C: Triggering nested event\n";
            Game::EventScope scope;
            mgr.invokeCallback("OnNestedEvent", damage);
        },
        30
    );

    mgr.addCallback("OnNestedEvent", [](int damage) {
        std::cout << "Nested callback: Damage = " << damage << "\n";
        // 这里调用 preventDefault 不会影响外层 OnTakeDamage 的状态
        Game::getInstance().preventDefault();
    });

    {
        Game::EventScope scope;
        mgr.invokeCallback("OnTakeDamage", 200);
        std::cout << "Outer scope prevented: " << (scope.isPrevented() ? "Yes" : "No") << std::endl;
    }

    {
        Game::EventScope scope;
        mgr.invokeCallback("OnTakeDamage", 20);
        std::cout << "Outer scope prevented: " << (scope.isPrevented() ? "Yes" : "No") << std::endl;
    }

    return true;
}

bool PrimordialCore::enable() {
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool PrimordialCore::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

bool PrimordialCore::unload() {
    getSelf().getLogger().debug("Unloading...");
    // Code for unloading the mod goes here.
    return true;
}

} // namespace primordial_core

LL_REGISTER_MOD(primordial_core::PrimordialCore, primordial_core::PrimordialCore::getInstance());
