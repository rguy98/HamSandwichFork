#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <string>
#include <unistd.h>  // for chdir
#include "globals.h"
#include "editor.h"
#include "resources.rc.h"

using namespace std;

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *gFont, *gIconFont;

int DISPLAY_WIDTH = 1024;
int DISPLAY_HEIGHT = 768;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);
    if (TTF_Init()) printf("TTF_Init: %s\n", TTF_GetError());

    /*string exe = argv[0];
    string dir = exe.substr(0, exe.find_last_of("\\/"));
    chdir(dir.c_str());*/

    gFont = TTF_OpenFontRW(EmbeddedRW(vera), true, 14);
    if (!gFont) printf("vera.ttf: %s\n", TTF_GetError());
    gIconFont = TTF_OpenFontRW(EmbeddedRW(fontawesome), true, 14);
    if (!gIconFont) printf("fontawesome.ttf: %s\n", TTF_GetError());

    SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_GetWindowSize(window, &DISPLAY_WIDTH, &DISPLAY_HEIGHT);
    SDL_SetWindowTitle(window, "JspEdit 3");

    SDL_Surface *surface = IMG_Load_RW(EmbeddedRW(allegro_icon), true);
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);

    if (argc > 1) {
        editor::loadOnStartup(argv[1]);
    }

    editor::main();

    TTF_CloseFont(gIconFont);
    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

void DrawText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, Align align, SDL_Color color, const char* text) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) {
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = { x, y, surface->w, surface->h };
    if (align == ALIGN_RIGHT)
        rect.x -= surface->w;
    else if (align == ALIGN_CENTER)
        rect.x -= surface->w / 2;

    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
