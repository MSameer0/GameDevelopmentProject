#include "Game.h"
#include <iostream>

bool Game::init() {
    return true;
}

void Game::update(float deltaTime) {
    std::cout << "Current delta: " << deltaTime << std::endl;
}

void Game::destroy() {

}
