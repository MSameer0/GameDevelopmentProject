#pragma once
#include <memory>
#include <chrono>

namespace eng {
    class Application;
    class Engine {
        std::unique_ptr<Application> m_application;
        std::chrono::steady_clock::time_point m_lastTimePoint;
    public:
        bool init();
        void run();
        void destroy();

        void setApplication(Application* app);
        Application* getApplication();
    };
}