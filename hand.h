#ifndef _HAND_
#define _HAND_
#include <iostream>
#include "game.h"
#include "gameplay.h"
#include "graphics.h"
#include "zones.h"
class hand
{
public:
    void draw(Graphics &graphics, zones &zones, Gameplay &gameplay,int turn)
    {
        int startx1,startx2;
        startx1=300+gameplay.playerHand.size()*71;
        startx2=300+gameplay.opponentHand.size()*71;
        if(turn%2==1)
        {
            if(turn!=1)
            {
                gameplay.playerDrawCard();
                gameplay.renderSingleCardOnHand(graphics, zones, gameplay.playerHand.back(), startx1, starty1);
            }
        }
        else
        {
            gameplay.opponentDrawCard();
            gameplay.opponentrenderSingleCardOnHand(graphics, zones, gameplay.opponentHand.back(), startx2, starty2);
        }
        SDL_Delay(300);
        graphics.presentScene();
    }
};
#endif // _HAND_
