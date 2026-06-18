#pragma once
#include <memory>
#include <chrono>

#include "graphics/GraphicsAPI.h"
#include "input/InputManager.h"
#include "render/RenderQueue.h"

struct GLFWwindow;
namespace eng {
    class Application;
    class Engine {
        GLFWwindow* m_window = nullptr;
        std::unique_ptr<Application> m_application;
        std::chrono::steady_clock::time_point m_lastTimePoint;

        InputManager m_inputManager;
        GraphicsAPI m_graphicsAPI;

        RenderQueue m_renderQueue;

        Engine() = default;
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
    public:
        static Engine& getInstance();

        bool init(int w, int h);
        void run();
        void destroy();

        void setApplication(Application* app);
        Application* getApplication();

        InputManager& getInputManager();
        GraphicsAPI& getGraphicsAPI();

        RenderQueue& getRenderQueue();
    };
}