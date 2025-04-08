#ifndef _CARD_H
#define _CARD_H

#include <string>
#include <vector>
#include <SDL.h>
#include "graphics.h"
#include "zones.h"

enum class CardType { Monster };

struct Card
{
    std::string name;
    CardType type;
    int atk = 0;
    int def = 0;
    std::string imagePath;

    Card(std::string n, int atk, int def, std::string img)
        : name(n), type(CardType::Monster), atk(atk), def(def), imagePath(img) {}
    Card()
        : name(""), type(CardType::Monster), atk(0), def(0), imagePath("") {}
};



#endif // _CARD_H
