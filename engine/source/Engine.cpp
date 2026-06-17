#include "Engine.h"
#include "Application.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

namespace eng {
    void keyCallBack(GLFWwindow* window, int key, int, int action, int) {
        auto& inputManager = eng::Engine::getInstance().getInputManager();
        if (action == GLFW_PRESS) {
            inputManager.setKeyPressed(key, true);
        } else if (action == GLFW_RELEASE) {
            inputManager.setKeyPressed(key, false);
        }
    }

    Engine& Engine::getInstance() {
        static Engine instance;
        return instance;
    }

    bool Engine::init(int w, int h) {
        if (!m_application) {
            return false;
        }

        if (!glfwInit()) {
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(w, h, "Game", nullptr, nullptr);

        if (m_window == nullptr) {
            std::cout << "Failed to create GLFW window." << std::endl;
            glfwTerminate();
            return false;
        }

        glfwSetKeyCallback(m_window, keyCallBack);

        glfwMakeContextCurrent(m_window);

        if (glewInit() != GLEW_OK) {
            glfwTerminate();
            return false;
        }

        return m_application->init();
    }

    void Engine::run() {
        if (!m_application) {
            return;
        }

        m_lastTimePoint = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(m_window) && !m_application->needsToBeClosed()) {
            glfwPollEvents();

            auto now = std::chrono::steady_clock::now();
            float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
            m_lastTimePoint = now;

            m_application->update(deltaTime);

            glfwSwapBuffers(m_window);
        }
    }

    void Engine::destroy() {
        if (m_application) {
            m_application->destroy();
            m_application.reset();

            glfwTerminate();
            m_window = nullptr;
        }
    }

    void Engine::setApplication(Application *app) {
        m_application.reset(app);
    }

    Application* Engine::getApplication() {
        return m_application.get();
    }

    InputManager& Engine::getInputManager() {
        return m_inputManager;
    }

    GraphicsAPI& Engine::getGraphicsAPI() {
        return m_graphicsAPI;
    }
}
