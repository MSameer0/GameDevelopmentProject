#include "Game.h"
#include <eng.h>

int main() {
    Game* game  = new Game();
    eng::Engine engine;

    engine.setApplication(game);

    if (engine.init(1280, 720)) {
        engine.run();
    }

    engine.destroy();
    return 0;
}