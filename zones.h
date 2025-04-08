#ifndef _ZONES_
#define _ZONES_
#include <SDL.h>
#include <vector> // Đảm bảo thêm thư viện vector

using namespace std; // Để sử dụng các thành phần trong không gian tên std mà không cần std::

class zones
{
public:
    vector<SDL_Rect> monsterZonesP1; // Quái thú của người chơi 1
    vector<SDL_Rect> monsterZonesP2; // Quái thú của người chơi 2

    vector<SDL_Rect> spellTrapZonesP1; // Phép/trap của người chơi 1
    vector<SDL_Rect> spellTrapZonesP2; // Phép/trap của người chơi 2

    vector<SDL_Rect> handZonesP1; // Bài trên tay của người chơi 1
    vector<SDL_Rect> handZonesP2; // Bài trên tay của người chơi 2
    SDL_Rect playerDeckZone = {250 + 81*5,175+122*3, CARD_WIDTH, CARD_HEIGHT};
    SDL_Rect opponentDeckZone = {250 + 81*5,175, CARD_WIDTH, CARD_HEIGHT};
    SDL_Rect playerGraveyardZone = {250 + 81*5,175+122*2, CARD_WIDTH, CARD_HEIGHT};
    SDL_Rect opponentGraveyardZone = {250 + 81*5,175+122, CARD_WIDTH, CARD_HEIGHT};;
};
#endif
