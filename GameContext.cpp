#include "GameContext.h"

void GameContext::init() {
    graphics.init();
    gameUI = new GameUI(graphics, zone);
    gameUI->render();
    initHand.start(graphics, zone, gameplay);
}

void GameContext::cleanup() {
    graphics.quit();
    if (gameUI) {
        delete gameUI;
        gameUI = nullptr;
    }
}

