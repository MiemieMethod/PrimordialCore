#include "pc/world/Game.h"

namespace primordial_core {

thread_local std::vector<bool> Game::prevent_stack;

Game& Game::getInstance() {
    static Game instance;
    return instance;
}

void Game::preventDefault() {
    if (!prevent_stack.empty()) {
        prevent_stack.back() = true;
    }
}

bool Game::_isDefaultPrevented() const {
    if (prevent_stack.empty()) return false;
    return prevent_stack.back();
}

Game::EventScope::EventScope() { Game::prevent_stack.push_back(false); }

Game::EventScope::~EventScope() { Game::prevent_stack.pop_back(); }

bool Game::EventScope::isPrevented() const { return Game::getInstance()._isDefaultPrevented(); }

} // namespace primordial_core
