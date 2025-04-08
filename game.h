#ifndef _GAME_UI_H
#define _GAME_UI_H

#include "graphics.h"
#include "defs.h"
#include "card.h"
#include "deck.h"
#include "zones.h"
// Lớp quản lý giao diện trò chơi
struct GameUI
{
    Graphics &graphics; // Đối tượng xử lý đồ họa
    SDL_Texture *tableTexture; // Hình nền bàn chơi
    // Kích thước màn hình

    // Kích thước lá bài

    // Điều chỉnh vị trí giữ bài để không bị đè lên sân chơi


    // Mảng chứa vị trí các ô bài trên sân (5 cột, 4 hàng, tổng cộng 20 ô)
    SDL_Rect playArea[24];
    // chứa hình các ô
    SDL_Texture *cardTextures[24];
    // Constructor khởi tạo giao diện
    GameUI(Graphics &gfx,zones &zones) : graphics(gfx)
    {
        tableTexture = graphics.loadTexture("images//background.jpg"); // Tải hình nền bàn chơi
        initPlayArea(zones); // Thiết lập vị trí các ô bài trên sân
    }

    // Destructor giải phóng tài nguyên
    ~GameUI()
    {
        SDL_DestroyTexture(tableTexture);
    }
    // Thiết lập vị trí các ô bài trên sân (5 ngang, 4 dọc)
    void initPlayArea(zones &zones)
    {
        int startX = 250; // Điểm bắt đầu theo chiều ngang
        int startY = 175; // Điểm bắt đầu theo chiều dọc
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 6; col++)
            {
                int index = row * 6 + col;
                playArea[index] = {startX + col * (CARD_WIDTH + 10), startY + row * (CARD_HEIGHT + 20), CARD_WIDTH, CARD_HEIGHT};
                if(col<5)
                {
                    if(row==0) zones.spellTrapZonesP2.push_back({startX + col * (CARD_WIDTH + 10), startY + row * (CARD_HEIGHT + 20), CARD_WIDTH, CARD_HEIGHT});
                    if(row==1) zones.monsterZonesP2.push_back({startX + col * (CARD_WIDTH + 10), startY + row * (CARD_HEIGHT + 20), CARD_WIDTH, CARD_HEIGHT});
                    if(row==2) zones.monsterZonesP1.push_back({startX + col * (CARD_WIDTH + 10), startY + row * (CARD_HEIGHT + 20), CARD_WIDTH, CARD_HEIGHT});
                    if(row==3) zones.spellTrapZonesP1.push_back({startX + col * (CARD_WIDTH + 10), startY + row * (CARD_HEIGHT + 20), CARD_WIDTH, CARD_HEIGHT});
                }
            }
        }

    }

    // Vẽ giao diện trò chơi
    void render()
    {
        graphics.prepareScene(tableTexture); // Vẽ nền bàn chơi
        SDL_SetRenderDrawColor(graphics.renderer, 225, 225, 225, 255);
        for (int i = 0; i < 24; i++)
        {
            if (i == 11 || i == 17)
            {
                // Vẽ dấu gạch chéo
                // Vẽ đường chéo từ góc trên bên trái đến góc dưới bên phải
                SDL_RenderDrawLine(graphics.renderer, playArea[i].x, playArea[i].y,
                                   playArea[i].x + playArea[i].w, playArea[i].y + playArea[i].h);

                // Vẽ đường chéo từ góc dưới bên trái đến góc trên bên phải
                SDL_RenderDrawLine(graphics.renderer, playArea[i].x, playArea[i].y + playArea[i].h,
                                   playArea[i].x + playArea[i].w, playArea[i].y);
            }
            SDL_RenderDrawRect(graphics.renderer, &playArea[i]);

        }

        graphics.presentScene(); // Cập nhật hiển thị
    }
    void handleMouseMotion(SDL_Renderer* renderer, zones& zones, int &lastHoveredZone, int mouseX, int mouseY)
    {
        int currentZone = -1;
        SDL_Point point = { mouseX, mouseY }; // Tạo một SDL_Point tạm thời
        SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
        SDL_RenderDrawRect(renderer, &zones.monsterZonesP1[lastHoveredZone]);
        SDL_RenderDrawRect(renderer, &zones.spellTrapZonesP1[lastHoveredZone]);
        SDL_RenderDrawRect(renderer, &zones.monsterZonesP2[lastHoveredZone]);
        SDL_RenderDrawRect(renderer, &zones.spellTrapZonesP2[lastHoveredZone]);
        SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255);
        // Kiểm tra tất cả các vùng quái thú và phép/trap của người chơi 1 và 2
        for (size_t i = 0; i < zones.monsterZonesP1.size(); ++i)
        {
            if (SDL_PointInRect(&point, &zones.monsterZonesP1[i]))
            {
                currentZone = i;
                SDL_RenderDrawRect(renderer, &zones.monsterZonesP1[i]);
                break;
            }
        }

        if (currentZone == -1)
        {
            for (size_t i = 0; i < zones.spellTrapZonesP1.size(); ++i)
            {
                if (SDL_PointInRect(&point, &zones.spellTrapZonesP1[i]))
                {
                    currentZone = i;
                    SDL_RenderDrawRect(renderer, &zones.spellTrapZonesP1[lastHoveredZone]);
                    break;
                }
            }
        }

        if (currentZone == -1)
        {
            for (size_t i = 0; i < zones.monsterZonesP2.size(); ++i)
            {
                if (SDL_PointInRect(&point, &zones.monsterZonesP2[i]))
                {
                    currentZone = i;
                    SDL_RenderDrawRect(renderer, &zones.monsterZonesP2[i]);
                    break;
                }
            }
        }

        if (currentZone == -1)
        {
            for (size_t i = 0; i < zones.spellTrapZonesP2.size(); ++i)
            {
                if (SDL_PointInRect(&point, &zones.spellTrapZonesP2[i]))
                {
                    currentZone = i;
                    SDL_RenderDrawRect(renderer, &zones.spellTrapZonesP2[i]);
                    break;
                }
            }
        }
        if (currentZone == -1)
        {
            for (size_t i = 0; i < zones.handZonesP1.size(); ++i)
            {
                if (SDL_PointInRect(&point, &zones.handZonesP1[i]))
                {
                    currentZone = i;
                    SDL_RenderDrawRect(renderer, &zones.handZonesP1[i]);
                    break;
                }
            }
        }
        if (currentZone == -1)
        {
            for (size_t i = 0; i < zones.handZonesP2.size(); ++i)
            {
                if (SDL_PointInRect(&point, &zones.handZonesP2[i]))
                {
                    currentZone = i;
                    SDL_RenderDrawRect(renderer, &zones.handZonesP2[i]);
                    break;
                }
            }
        }
        // Cập nhật zone hiện tại
        lastHoveredZone = currentZone;
    }

};

#endif // _GAME_UI_H
