#ifndef GUI_H
#define GUI_H

#include "globals.h"
#include "fontawesome.h"
#include <functional>
#include <vector>

inline bool rect_contains(SDL_Rect r, int x, int y) {
    return r.x <= x && x < r.x + r.w && r.y <= y && y < r.y + r.h;
}

struct KbdShortcut {
    int mods, keycode;

    bool operator==(const KbdShortcut& other) {
        return mods == other.mods && keycode == other.keycode;
    };
    bool operator!=(const KbdShortcut& other) { return !(*this == other); }
};

class Gui {
    int mx, my;
    bool justClicked;
    KbdShortcut justTyped;
    std::string tooltip;

public:
    Gui();

    void render();
    bool handleEvent(const SDL_Event &evt);

    bool element(SDL_Rect rect, TTF_Font *font, const std::string &text, const std::string &desc, KbdShortcut shortcut);
    bool button(SDL_Rect rect, const std::string &text, const std::string &desc, KbdShortcut shortcut);
    bool iconButton(int x, int y, FAChar ch, const std::string &desc, KbdShortcut shortcut);
};

#endif // GUI_H
