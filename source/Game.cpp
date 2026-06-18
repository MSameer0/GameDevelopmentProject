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

    m_material.setShaderProgram(shaderProgram);

    std::vector<float> vertices = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    eng::VertexLayout vertexLayout;

    //position
    vertexLayout.elements.push_back({
        0,
        3,
        GL_FLOAT,
        0
    });

    //color
    vertexLayout.elements.push_back({
        1,
        3,
        GL_FLOAT,
        sizeof(float) * 3
    });

    vertexLayout.stride = sizeof(float) * 6;

    m_mesh = std::make_unique<eng::Mesh>(vertexLayout, vertices, indices);

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
