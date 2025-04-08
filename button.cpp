#include "Button.h"

Button::Button(int x, int y, SDL_Texture* tex) {
    texture = tex;
    rect.x = x;
    rect.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

void Button::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

bool Button::isClicked(int mouseX, int mouseY) {
    SDL_Point point = {mouseX, mouseY};
    return SDL_PointInRect(&point, &rect);
}

