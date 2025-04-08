#ifndef _RENDERING_H
#define _RENDERING_H

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include "graphics.h"
#include "zones.h"
#include "card.h" // Card struct & enum

// Vẽ bài trên tay người chơi
inline void renderPlayerHand(Graphics& graphics, const std::vector<Card>& hand, zones& zones)
{
    for (size_t i = 0; i < hand.size(); ++i)
    {
        SDL_Texture* tex = graphics.loadTexture(hand[i].imagePath.c_str());
        SDL_Rect dst = zones.handZonesP1[i];
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}

// Vẽ bài trên tay đối thủ (mặt sau)
inline void renderOpponentHand(Graphics& graphics, const std::vector<Card>& hand, zones& zones)
{
    for (size_t i = 0; i < hand.size(); ++i)
    {
        SDL_Texture* tex = graphics.loadTexture("images//Mặt_sau_lá_bài.jpg");
        SDL_Rect dst = zones.handZonesP2[i];
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}

// Vẽ các quái vật trên sân người chơi
inline void renderMonstersOnField(Graphics& graphics, const std::vector<Card>& monsters, const std::vector<SDL_Rect>& monsterZones)
{
    for (size_t i = 0; i < monsters.size(); ++i)
    if(monsters[i].name!="")
    {
        // Kiểm tra rect hợp lệ
        SDL_Texture* tex = graphics.loadTexture(monsters[i].imagePath.c_str());
        SDL_Rect dst = monsterZones[i];
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}

// Vẽ các quái vật trên sân đối thủ (mặt sau)
inline void renderOpponentMonsters(Graphics& graphics, const std::vector<Card>& monsters, const std::vector<SDL_Rect>& monsterZones)
{
    for (size_t i = 0; i < monsters.size(); ++i)
    if(monsters[i].name!="")
    {
        SDL_Texture* tex = graphics.loadTexture(monsters[i].imagePath.c_str());
        SDL_Rect dst = monsterZones[i];
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}

// Vẽ viền các zone trên sân
inline void renderZoneOutlines(Graphics& graphics, const zones& zones)
{
    SDL_SetRenderDrawColor(graphics.renderer, 225, 225, 225, 255);
    for (const auto& rect : zones.monsterZonesP1)
        SDL_RenderDrawRect(graphics.renderer, &rect);
    for (const auto& rect : zones.monsterZonesP2)
        SDL_RenderDrawRect(graphics.renderer, &rect);
    for (const auto& rect : zones.spellTrapZonesP1)
        SDL_RenderDrawRect(graphics.renderer, &rect);
    for (const auto& rect : zones.spellTrapZonesP2)
        SDL_RenderDrawRect(graphics.renderer, &rect);
}
inline void renderDecks(Graphics& graphics, const zones& zones)
{
    SDL_Texture* tex = graphics.loadTexture("images//Mặt_sau_lá_bài.jpg");
    SDL_RenderCopy(graphics.renderer, tex, nullptr, &zones.playerDeckZone);
    SDL_RenderCopy(graphics.renderer, tex, nullptr, &zones.opponentDeckZone);
    SDL_DestroyTexture(tex);
}

inline void renderGraveyards(Graphics& graphics,
                             const std::vector<Card>& playerGraveyard,
                             const std::vector<Card>& opponentGraveyard,
                             const SDL_Rect& playerGraveyardZone,
                             const SDL_Rect& opponentGraveyardZone)
{
    // --- Player Graveyard ---
    if (!playerGraveyard.empty())
    {
        SDL_Texture* tex = graphics.loadTexture(playerGraveyard.back().imagePath.c_str());
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &playerGraveyardZone);
        SDL_DestroyTexture(tex);
    }
    else
    {
        // Vẽ hình chữ nhật trống
        SDL_SetRenderDrawColor(graphics.renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(graphics.renderer, &playerGraveyardZone);

        // Gạch chéo
        SDL_RenderDrawLine(graphics.renderer,
                           playerGraveyardZone.x, playerGraveyardZone.y,
                           playerGraveyardZone.x + playerGraveyardZone.w,
                           playerGraveyardZone.y + playerGraveyardZone.h);
        SDL_RenderDrawLine(graphics.renderer,
                           playerGraveyardZone.x + playerGraveyardZone.w, playerGraveyardZone.y,
                           playerGraveyardZone.x, playerGraveyardZone.y + playerGraveyardZone.h);
    }

    // --- Opponent Graveyard ---
    if (!opponentGraveyard.empty())
    {
        SDL_Texture* tex = graphics.loadTexture(opponentGraveyard.back().imagePath.c_str());
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &opponentGraveyardZone);
        SDL_DestroyTexture(tex);
    }
    else
    {
        SDL_SetRenderDrawColor(graphics.renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(graphics.renderer, &opponentGraveyardZone);

        SDL_RenderDrawLine(graphics.renderer,
                           opponentGraveyardZone.x, opponentGraveyardZone.y,
                           opponentGraveyardZone.x + opponentGraveyardZone.w,
                           opponentGraveyardZone.y + opponentGraveyardZone.h);
        SDL_RenderDrawLine(graphics.renderer,
                           opponentGraveyardZone.x + opponentGraveyardZone.w, opponentGraveyardZone.y,
                           opponentGraveyardZone.x, opponentGraveyardZone.y + opponentGraveyardZone.h);
    }
}
void renderFullScene(Graphics& graphics, zones& zones,
                     std::vector<Card>& playerField,
                     std::vector<Card>& opponentField,
                     std::vector<Card>& playerGraveyard,
                     std::vector<Card>& opponentGraveyard,
                     std::vector<Card>& playerHand,
                     std::vector<Card>& opponentHand,
                     SDL_Texture* background)
{
    graphics.prepareScene(background); // nền trước

    renderZoneOutlines(graphics, zones);
    renderDecks(graphics, zones);
    renderGraveyards(graphics, playerGraveyard, opponentGraveyard, zones.playerGraveyardZone, zones.opponentGraveyardZone);
    renderMonstersOnField(graphics, playerField, zones.monsterZonesP1);
    renderOpponentMonsters(graphics, opponentField, zones.monsterZonesP2);
    renderPlayerHand(graphics, playerHand, zones);
    renderOpponentHand(graphics, opponentHand, zones);

    graphics.presentScene(); // cuối cùng
}
// Hàm chính: vẽ toàn bộ bài và quái vật, zone

#endif // _RENDERING_H


