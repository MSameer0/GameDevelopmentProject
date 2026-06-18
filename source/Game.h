#pragma once
#include <eng.h>
#include <memory>

class Game: public eng::Application {
    eng::Material m_material;
    std::unique_ptr<eng::Mesh> m_mesh;
public:
    bool init() override;
    void update(float deltaTime) override;
    void destroy() override;
};