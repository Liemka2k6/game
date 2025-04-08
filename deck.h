#ifndef _DECK_H
#define _DECK_H
#include <algorithm>
#include "Card.h"
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
class Deck {
public:
    std::vector<Card> cards;
    Deck() {
        cards.push_back(Card("Blue-Eyes White Dragon", 3000, 2500, "images//blue_eyes.jpg"));
        cards.push_back(Card("Dark Magician", 2500, 2100, "images//dark_magician.jpg"));
        cards.push_back(Card("Red-Eyes Black Dragon", 2400, 2000, "images//red_eyes.jpg"));
        cards.push_back(Card("Summoned Skull", 2500, 1200, "images//summoned_skull.jpg"));
        shuffleDeck();
    }
    void shuffleDeck() {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        std::shuffle(cards.begin(), cards.end(), g);
    }

};

#endif // _DECK_H
