#pragma once
#include <eng.h>

class Game: public eng::Application {
public:
    bool init() override;
    void update(float deltaTime) override;
    void destroy() override;
};