#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "graphics.h"
#include "gameplay.h"
#include "game.h"
#include "initiallyhand.h"
#include "hand.h"
#include "zones.h"
#include "menu.h"
using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if (SDL_PollEvent(&e) != 0 &&
                (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
    }
}
bool isInsideCircle(int x, int y, int centerX, int centerY, int radius)
{
    int dx = x - centerX;
    int dy = y - centerY;
    return (dx * dx + dy * dy) <= (radius * radius);
}

// Hàm vẽ nút hình tròn "End Turn"
void renderEndTurnCircleButton(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    // Tô màu nền nút
    SDL_SetRenderDrawColor(renderer, 225, 0, 0, 255); // màu xanh dương
    for (int w = -radius; w <= radius; ++w)
    {
        for (int h = -radius; h <= radius; ++h)
        {
            if (w * w + h * h <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);
            }
        }
    }
}
int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    int menuResult = menu(graphics);
    if (menuResult != 1)
    {
        graphics.quit();
        return 0;
    }

    zones zones;
    GameUI gameUI(graphics, zones);
    gameUI.render();
    Gameplay gameplay;
    initiallyhand initiallyhand;
    initiallyhand.start(graphics, zones, gameplay);

    hand playerHand;
    int lastHoveredZone = -1;
    SDL_Event event;
    int x, y, turn = 1;
    Mix_Chunk* attackSound = nullptr;
    attackSound = Mix_LoadWAV("assets/attack.mp3");

    while (true) // vòng lặp chính
    {
        int clicked = -1, click = -1;
        playerHand.draw(graphics, zones, gameplay, turn);
        while (true)
        {
            SDL_GetMouseState(&x, &y);
            renderEndTurnCircleButton(graphics.renderer, endButtonX, endButtonY, endButtonRadius);
            gameUI.handleMouseMotion(graphics.renderer, zones, lastHoveredZone, x, y);
            if (turn % 2 == 1) // lượt người chơi
            {
                SDL_PollEvent(&event);
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    if (clicked == -1 && gameplay.clickhand(x, y, zones) != -1)
                    {
                        click = gameplay.clickhand(x, y, zones);
                        clicked = 0;
                    }
                    else if (clicked == 0 && gameplay.clickfield(x, y, zones) != -1)
                    {
                        int click1=gameplay.clickfield(x, y, zones);
                        gameplay.handleSummonClick(x,y,click,click1,zones,graphics);
                        clicked = 1;
                    }
                    if(turn!=1) gameplay.handleAttackClick(x, y, zones, graphics, attackSound);
                    if (isInsideCircle(x, y, endButtonX, endButtonY, endButtonRadius)) break;
                }

            }
            else
            {
                gameplay.opponentremoveCardWithAnimation(zones, graphics);
                break;
            }

            graphics.presentScene();
        }
        turn++;

    }

    waitUntilKeyPressed();
    Mix_FreeChunk(attackSound);
    attackSound = nullptr;
    graphics.quit();
    return 0;
}
