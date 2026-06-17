#include "Game.h"
#include <iostream>
#include <GLFW/glfw3.h>

bool Game::init() {
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        void main() {
            vColor = color;
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
    )";

    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;

        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    auto& graphicsAPI = eng::Engine::getInstance().getGraphicsAPI();
    auto shaderProgram = graphicsAPI.createShaderProgram(vertexShaderSource, fragmentShaderSource);

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
