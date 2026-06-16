#pragma once
#include <memory>
#include <chrono>

struct GLFWwindow;
namespace eng {
    class Application;
    class Engine {
        GLFWwindow* m_window = nullptr;
        std::unique_ptr<Application> m_application;
        std::chrono::steady_clock::time_point m_lastTimePoint;
    public:
        bool init(int w, int h);
        void run();
        void destroy();

        void setApplication(Application* app);
        Application* getApplication();
    };
}