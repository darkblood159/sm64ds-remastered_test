//Improved Mouse controls with DirectX support by sm64coopdx team.
#ifdef MOUSE_ACTIONS

#include "controller_mouse.h"
#include "../configfile.h"

#ifdef WAPI_DXGI
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
extern HWND gfx_dxgi_get_h_wnd(void);
extern void gfx_dxgi_set_cursor_visibility(bool visible);
#elif defined(CAPI_SDL1)
#include <SDL/SDL.h>
#elif defined(CAPI_SDL2)
#include <SDL2/SDL.h>
#endif

int mouse_prev_window_x;
int mouse_prev_window_y;

int mouse_has_center_control;
int mouse_has_current_control;

bool mouse_init_ok;

u32 mouse_buttons;
s32 mouse_x;
s32 mouse_y;

u32 mouse_window_buttons;
s32 mouse_window_x;
s32 mouse_window_y;

bool mouse_relative_enabled;

#ifdef WAPI_DXGI
u32 mouse_relative_buttons_held_on_focus;
u32 mouse_window_buttons_held_on_focus;
bool mouse_dxgi_prev_focus;

static u32 controller_mouse_dxgi_button_state(u32* mouse_held, bool has_focus) {
    // TODO: Test extra mouse buttons if possible (VK_XBUTTON1, VK_XBUTTON2).
    u32 mouse =
        ((GetKeyState(VK_LBUTTON) < 0) ? (1 << 0) : 0) |
        ((GetKeyState(VK_MBUTTON) < 0) ? (1 << 1) : 0) |
        ((GetKeyState(VK_RBUTTON) < 0) ? (1 << 2) : 0);

    bool prev_focus = mouse_dxgi_prev_focus;
    mouse_dxgi_prev_focus = has_focus;

    // Ignore mouse clicks when game window doesn't have focus.
    if (!has_focus) { return 0; }
    if (mouse_held == NULL) { return mouse; }

    // Window just received input focus, ignore any held down mouse buttons.
    if (!prev_focus && has_focus) {
        *mouse_held = mouse;
        return 0;
    }

    // Wait for a mouse button held down (e.g. while clicking on the window)
    // to be released and pressed again.
    *mouse_held = (*mouse_held) & mouse;
    return ~(*mouse_held) & mouse;
}
#endif // WAPI_DXGI

static void controller_mouse_is_visible(int visible) {
#if defined(WAPI_DXGI)
    gfx_dxgi_set_cursor_visibility(visible);
#elif defined(CAPI_SDL1) || defined(CAPI_SDL2)
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
#endif
}

void controller_mouse_set_visible(void) {
    if (configWindow.fullscreen) {
        controller_mouse_is_visible(false);
    } else {
        controller_mouse_is_visible(!mouse_has_current_control);
    }
}

int controller_mouse_set_position(void *cursorX, void *cursorY, f32 mPosX, f32 mPosY, int hasControlCondition, int isInteger) {
    // Disable mouse control if condition isn't met.
    if (!hasControlCondition) {
        mouse_has_current_control = FALSE;
    }

    // Check if the mouse moved and update control status if condition is met.
    if ((mouse_window_x - mouse_prev_window_x != 0 || mouse_window_y - mouse_prev_window_y != 0) && hasControlCondition) {
        mouse_has_current_control = TRUE;
    }

    // Scale calculations and cursor position update if the mouse has control.
    if (mouse_has_current_control) {
        float posX = mPosX;
        float posY = mPosY;

        // Update position based on type
        if (isInteger) { // Mario Head cursor
            *(s32*)cursorX = (s32)posX;
            *(s32*)cursorY = (s32)posY;
        } else { // Float, File select cursor
            *(float*)cursorX = posX;
            *(float*)cursorY = posY;
        }
    }

    // Make the controller mouse visible and update the previous mouse position.
    controller_mouse_set_visible();
    mouse_prev_window_x = mouse_window_x;
    mouse_prev_window_y = mouse_window_y;

    return mouse_has_current_control;
}

void controller_mouse_read_window(void) {
    if (!mouse_init_ok) { return; }

#if defined(WAPI_DXGI)
    HWND game_window = gfx_dxgi_get_h_wnd();

    mouse_window_buttons = controller_mouse_dxgi_button_state(
        &mouse_window_buttons_held_on_focus,
        GetFocus() == game_window);

    POINT p;
    if (GetCursorPos(&p) && ScreenToClient(game_window, &p)) {
        mouse_window_x = p.x;
        mouse_window_y = p.y;
    }
#elif defined(CAPI_SDL1) || defined(CAPI_SDL2)
    mouse_window_buttons = SDL_GetMouseState(&mouse_window_x, &mouse_window_y);
#endif
}

void controller_mouse_read_relative(void) {
    if (!mouse_init_ok) { return; }

#if defined(WAPI_DXGI)
    HWND game_window = gfx_dxgi_get_h_wnd();

    // Always get the buttons state, regardless if the relative mode is enabled.
    mouse_buttons = controller_mouse_dxgi_button_state(
        &mouse_relative_buttons_held_on_focus,
        GetFocus() == game_window);

    // Force mouse position to be on the center of the window.
    if (mouse_relative_enabled) {
        static POINT p0;
        POINT p1;
        RECT rect;
        if (GetWindowRect(gfx_dxgi_get_h_wnd(), &rect) && GetCursorPos(&p1)) {
            mouse_x = p1.x - p0.x;
            mouse_y = p1.y - p0.y;

            p0.x = rect.left + (rect.right - rect.left) / 2;
            p0.y = rect.top + (rect.bottom - rect.top) / 2;
            ClipCursor(&rect); // Prevents visible mouse if you move it too fast.
            SetCursorPos(p0.x, p0.y);
        }
    } else {
        ClipCursor(NULL);
        mouse_x = 0;
        mouse_y = 0;
    }

#elif defined(CAPI_SDL1) || defined(CAPI_SDL2)
    mouse_buttons = SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
#endif
}

void controller_mouse_enter_relative(void) {
    if (!mouse_relative_enabled) {
        mouse_relative_enabled = true;

#if defined(WAPI_DXGI)
        gfx_dxgi_set_cursor_visibility(false);
#elif defined(CAPI_SDL1)
        SDL_WM_GrabInput(SDL_GRAB_ON);
#elif defined(CAPI_SDL2)
        SDL_SetRelativeMouseMode(SDL_TRUE);
#endif
    }
}

void controller_mouse_leave_relative(void) {
    if (mouse_relative_enabled) {
        mouse_relative_enabled = false;

#if defined(WAPI_DXGI)
        gfx_dxgi_set_cursor_visibility(configWindow.fullscreen ? FALSE : TRUE);
#elif defined(CAPI_SDL1)
        SDL_WM_GrabInput(SDL_GRAB_OFF);
#elif defined(CAPI_SDL2)
        SDL_SetRelativeMouseMode(SDL_FALSE);
#endif
    }
}
#endif
