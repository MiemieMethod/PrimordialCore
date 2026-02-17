#pragma once

#include <vector>

namespace primordial_core {

class Game {
private:
    Game()                       = default;
    ~Game()                      = default;
    Game(const Game&)            = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&)                 = delete;
    Game& operator=(Game&&)      = delete;

    static thread_local std::vector<bool> prevent_stack;

public:
    static Game& getInstance();

    void preventDefault();
    bool _isDefaultPrevented() const;

    class EventScope {
    public:
        EventScope();
        ~EventScope();

        bool isPrevented() const;
    };
};

} // namespace primordial_core
