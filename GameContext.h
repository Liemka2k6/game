#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "graphics.h"
#include "zones.h"
#include "gameplay.h"
#include "initiallyhand.h"
#include "hand.h"
#include "game.h"

struct GameContext {
    Graphics graphics;
    zones zone;
    GameUI* gameUI = nullptr;
    Gameplay gameplay;
    initiallyhand initHand;
    hand playerHand;
    void init();
    void cleanup();
};

#endif
