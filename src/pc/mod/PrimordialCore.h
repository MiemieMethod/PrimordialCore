#pragma once

#include "ll/api/mod/NativeMod.h"

#include "pc/Macro.h"

namespace pc {

class PrimordialCore {

public:
    PCAPI static PrimordialCore& getInstance();

    PrimordialCore() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    [[nodiscard]] static ll::io::Logger& getLogger() { return getInstance().getSelf().getLogger(); }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // @return True if the mod is unloaded successfully.
    bool unload();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace pc
