#pragma once
#include <array>

namespace eng {
    class InputManager {
        InputManager() = default;
        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(InputManager&&) = delete;

        friend class Engine;
        std::array<bool, 256> m_keys = {false};
    public:
        void setKeyPressed(int key, bool pressed);
        bool isKeyPressed(int key);
    };
}