#ifndef _INITIALLYHAND_
#define _INITIALLYHAND_

#include "gameplay.h"  // Đảm bảo rằng lớp Gameplay đã được khai báo trước
#include "game.h"
#include "graphics.h"
#include "zones.h"

class initiallyhand
{
public:
    void start(Graphics &graphics, zones &zones, Gameplay &gameplay)
    {
        renderDecks(graphics, zones);
        renderGraveyards(graphics, gameplay.playerGraveyard, gameplay.opponentGraveyard, zones.playerGraveyardZone, zones.opponentGraveyardZone);
        int startx = 300;
        for (int i = 0; i < 3; i++)
        {
            gameplay.playerDrawCard();
            gameplay.opponentDrawCard();
            gameplay.renderSingleCardOnHand(graphics, zones, gameplay.playerHand[i], startx, starty1);
            gameplay.opponentrenderSingleCardOnHand(graphics, zones, gameplay.opponentHand[i], startx, starty2);
            graphics.presentScene();
            SDL_Delay(300);
            startx += CARD_WIDTH + 1;
        }
    }
};

#endif // _INITIALLYHAND_
