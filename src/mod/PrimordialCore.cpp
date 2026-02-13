#include "mod/PrimordialCore.h"
#include "callback/CallbackManager.hpp"

#include "ll/api/mod/RegisterHelper.h"

namespace primordial_core {

PrimordialCore& PrimordialCore::getInstance() {
    static PrimordialCore instance;
    return instance;
}

bool PrimordialCore::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.

    CallbackManager mgr;

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
