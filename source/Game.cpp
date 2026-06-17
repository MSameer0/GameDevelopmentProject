#include "Game.h"
#include <iostream>
#include <GLFW/glfw3.h>

bool Game::init() {
    return true;
}

void Game::update(float deltaTime) {
    auto& input = eng::Engine::getInstance().getInputManager();

    if (input.isKeyPressed(GLFW_KEY_A)) {
        std::cout << "[A] button is pressed" << std::endl;
    }
}

void Game::destroy() {

}
