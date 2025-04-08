#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

class Button {
public:
    SDL_Rect rect;
    SDL_Texture* texture;

    Button(int x, int y, SDL_Texture* tex);

    void render(SDL_Renderer* renderer);
    bool isClicked(int mouseX, int mouseY);
};

#endif // BUTTON_H
