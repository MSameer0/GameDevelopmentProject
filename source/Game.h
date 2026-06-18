#pragma once
#include <eng.h>

class Game: public eng::Application {
    eng::Material m_material;
public:
    bool init() override;
    void update(float deltaTime) override;
    void destroy() override;
};