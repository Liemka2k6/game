#include "menu.h"
#include "Graphics.h"
#include "Button.h"
#include "defs.h"  // Chứa SCREEN_WIDTH, SCREEN_HEIGHT
#include <SDL_mixer.h>

int menu(Graphics& graphics)
{
    // Load textures
    SDL_Texture* background = graphics.loadTexture("images//background.jpg");
    SDL_Texture* playButtonTex = graphics.loadTexture("images//play_button.png");
    SDL_Texture* quitButtonTex = graphics.loadTexture("images//exit_button.png");

    if (!background || !playButtonTex || !quitButtonTex)
    {
        SDL_Log("Failed to load one or more menu textures.");
        return 0; // Quit nếu load lỗi
    }

    // Button positioning
    int buttonWidth = 181;
    int centerX = (SCREEN_WIDTH - buttonWidth) / 2;

    // Create buttons
    Button playButton(centerX, 500, playButtonTex);
    Button quitButton(centerX, 600, quitButtonTex);

    SDL_Event event;
    bool running = true;
    int result = -1;
    Mix_Music* menuMusic = Mix_LoadMUS("assets/backgroundmusic.mp3");
    if (menuMusic != nullptr)
    {
        Mix_PlayMusic(menuMusic, -1);  // Lặp vô hạn
    }
    else
    {
        SDL_Log("Failed to load menu music: %s", Mix_GetError());
    }

// Load click sound
  /*  Mix_Chunk* clickSound = Mix_LoadWAV("assets/click_sound.wav");
    if (!clickSound)
    {
        SDL_Log("Failed to load click sound: %s", Mix_GetError());
    }*/
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                result = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (playButton.isClicked(x, y))
                {
                    // Dừng nhạc menu
                    Mix_HaltMusic();

                    // Phát nhạc game
                    Mix_Music* gameMusic = Mix_LoadMUS("assets/game_music.mp3");
                    if (gameMusic != nullptr)
                    {
                        Mix_PlayMusic(gameMusic, -1); // Lặp vô hạn
                    }
                    else
                    {
                        SDL_Log("Failed to load game music: %s", Mix_GetError());
                    }

                    result = 1;
                    running = false;
                }
                else if (quitButton.isClicked(x, y))
                {
                    result = 0;
                    running = false;
                }
            }
        }

        // Render menu
        graphics.prepareScene(background);
        playButton.render(graphics.renderer);
        quitButton.render(graphics.renderer);
        graphics.presentScene();

        SDL_Delay(16); // ~60fps
    }

    // Cleanup
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playButtonTex);
    SDL_DestroyTexture(quitButtonTex);

    return result;
}
