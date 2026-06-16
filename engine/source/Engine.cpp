#include "Engine.h"
#include "Application.h"

namespace eng {
    bool Engine::init() {
        if (!m_application) {
            return false;
        }
        return m_application->init();
    }

    void Engine::run() {
        if (!m_application) {
            return;
        }

        m_lastTimePoint = std::chrono::high_resolution_clock::now();
        while (!m_application->needsToBeClosed()) {
            auto now = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
            m_lastTimePoint = now;

            m_application->update(deltaTime);
        }
    }

    void Engine::destroy() {
        if (m_application) {
            m_application->destroy();
            m_application.reset();
        }
    }

    void Engine::setApplication(Application *app) {
        m_application.reset(app);
    }

    Application* Engine::getApplication() {
        return m_application.get();
    }
}
