#pragma once

#include <vector>

#include "pc/Macro.h"

namespace pc {

class Game {
private:
    PCAPI Game()                       = default;
    PCAPI ~Game()                      = default;
    PCAPI Game(const Game&)            = delete;
    PCAPI Game& operator=(const Game&) = delete;
    PCAPI Game(Game&&)                 = delete;
    PCAPI Game& operator=(Game&&)      = delete;

    static thread_local std::vector<bool> prevent_stack;

    bool _isDefaultPrevented() const;

public:
    PCAPI static Game& getInstance();

    PCAPI void preventDefault();

    class EventScope {
    public:
        PCAPI EventScope();
        PCAPI ~EventScope();

        PCAPI bool isPrevented() const;
    };
};

} // namespace pc
