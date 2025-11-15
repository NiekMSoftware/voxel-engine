// implementation generated with Claude.ai

#include "imgui_impl_x11.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <sys/time.h>
#include <cstring>

struct ImGui_ImplX11_Data {
    Display *Display;
    Window          Window;
    unsigned long   Time;
    Cursor          MouseCursors[ImGuiMouseCursor_COUNT];
    bool            MouseButtonsDown[5];
    ImGuiMouseCursor LastMouseCursor;

    ImGui_ImplX11_Data() { memset(this, 0, sizeof(*this)); }
};

static ImGui_ImplX11_Data *ImGui_ImplX11_GetBackendData() {
    return ImGui::GetCurrentContext() ? (ImGui_ImplX11_Data *)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

static ImGuiKey ImGui_ImplX11_KeyToImGuiKey(KeySym keysym) {
    switch (keysym)
    {
    case XK_Tab: return ImGuiKey_Tab;
    case XK_Left: return ImGuiKey_LeftArrow;
    case XK_Right: return ImGuiKey_RightArrow;
    case XK_Up: return ImGuiKey_UpArrow;
    case XK_Down: return ImGuiKey_DownArrow;
    case XK_Page_Up: return ImGuiKey_PageUp;
    case XK_Page_Down: return ImGuiKey_PageDown;
    case XK_Home: return ImGuiKey_Home;
    case XK_End: return ImGuiKey_End;
    case XK_Insert: return ImGuiKey_Insert;
    case XK_Delete: return ImGuiKey_Delete;
    case XK_BackSpace: return ImGuiKey_Backspace;
    case XK_space: return ImGuiKey_Space;
    case XK_Return: return ImGuiKey_Enter;
    case XK_Escape: return ImGuiKey_Escape;
    case XK_apostrophe: return ImGuiKey_Apostrophe;
    case XK_comma: return ImGuiKey_Comma;
    case XK_minus: return ImGuiKey_Minus;
    case XK_period: return ImGuiKey_Period;
    case XK_slash: return ImGuiKey_Slash;
    case XK_semicolon: return ImGuiKey_Semicolon;
    case XK_equal: return ImGuiKey_Equal;
    case XK_bracketleft: return ImGuiKey_LeftBracket;
    case XK_backslash: return ImGuiKey_Backslash;
    case XK_bracketright: return ImGuiKey_RightBracket;
    case XK_grave: return ImGuiKey_GraveAccent;
    case XK_Caps_Lock: return ImGuiKey_CapsLock;
    case XK_Scroll_Lock: return ImGuiKey_ScrollLock;
    case XK_Num_Lock: return ImGuiKey_NumLock;
    case XK_Print: return ImGuiKey_PrintScreen;
    case XK_Pause: return ImGuiKey_Pause;
    case XK_KP_0: return ImGuiKey_Keypad0;
    case XK_KP_1: return ImGuiKey_Keypad1;
    case XK_KP_2: return ImGuiKey_Keypad2;
    case XK_KP_3: return ImGuiKey_Keypad3;
    case XK_KP_4: return ImGuiKey_Keypad4;
    case XK_KP_5: return ImGuiKey_Keypad5;
    case XK_KP_6: return ImGuiKey_Keypad6;
    case XK_KP_7: return ImGuiKey_Keypad7;
    case XK_KP_8: return ImGuiKey_Keypad8;
    case XK_KP_9: return ImGuiKey_Keypad9;
    case XK_KP_Decimal: return ImGuiKey_KeypadDecimal;
    case XK_KP_Divide: return ImGuiKey_KeypadDivide;
    case XK_KP_Multiply: return ImGuiKey_KeypadMultiply;
    case XK_KP_Subtract: return ImGuiKey_KeypadSubtract;
    case XK_KP_Add: return ImGuiKey_KeypadAdd;
    case XK_KP_Enter: return ImGuiKey_KeypadEnter;
    case XK_KP_Equal: return ImGuiKey_KeypadEqual;
    case XK_Shift_L: return ImGuiKey_LeftShift;
    case XK_Control_L: return ImGuiKey_LeftCtrl;
    case XK_Alt_L: return ImGuiKey_LeftAlt;
    case XK_Super_L: return ImGuiKey_LeftSuper;
    case XK_Shift_R: return ImGuiKey_RightShift;
    case XK_Control_R: return ImGuiKey_RightCtrl;
    case XK_Alt_R: return ImGuiKey_RightAlt;
    case XK_Super_R: return ImGuiKey_RightSuper;
    case XK_Menu: return ImGuiKey_Menu;
    case XK_0: return ImGuiKey_0;
    case XK_1: return ImGuiKey_1;
    case XK_2: return ImGuiKey_2;
    case XK_3: return ImGuiKey_3;
    case XK_4: return ImGuiKey_4;
    case XK_5: return ImGuiKey_5;
    case XK_6: return ImGuiKey_6;
    case XK_7: return ImGuiKey_7;
    case XK_8: return ImGuiKey_8;
    case XK_9: return ImGuiKey_9;
    case XK_a: return ImGuiKey_A;
    case XK_b: return ImGuiKey_B;
    case XK_c: return ImGuiKey_C;
    case XK_d: return ImGuiKey_D;
    case XK_e: return ImGuiKey_E;
    case XK_f: return ImGuiKey_F;
    case XK_g: return ImGuiKey_G;
    case XK_h: return ImGuiKey_H;
    case XK_i: return ImGuiKey_I;
    case XK_j: return ImGuiKey_J;
    case XK_k: return ImGuiKey_K;
    case XK_l: return ImGuiKey_L;
    case XK_m: return ImGuiKey_M;
    case XK_n: return ImGuiKey_N;
    case XK_o: return ImGuiKey_O;
    case XK_p: return ImGuiKey_P;
    case XK_q: return ImGuiKey_Q;
    case XK_r: return ImGuiKey_R;
    case XK_s: return ImGuiKey_S;
    case XK_t: return ImGuiKey_T;
    case XK_u: return ImGuiKey_U;
    case XK_v: return ImGuiKey_V;
    case XK_w: return ImGuiKey_W;
    case XK_x: return ImGuiKey_X;
    case XK_y: return ImGuiKey_Y;
    case XK_z: return ImGuiKey_Z;
    case XK_F1: return ImGuiKey_F1;
    case XK_F2: return ImGuiKey_F2;
    case XK_F3: return ImGuiKey_F3;
    case XK_F4: return ImGuiKey_F4;
    case XK_F5: return ImGuiKey_F5;
    case XK_F6: return ImGuiKey_F6;
    case XK_F7: return ImGuiKey_F7;
    case XK_F8: return ImGuiKey_F8;
    case XK_F9: return ImGuiKey_F9;
    case XK_F10: return ImGuiKey_F10;
    case XK_F11: return ImGuiKey_F11;
    case XK_F12: return ImGuiKey_F12;
    default: return ImGuiKey_None;
    }
}

static void ImGui_ImplX11_UpdateMouseCursor() {
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplX11_Data *bd = ImGui_ImplX11_GetBackendData();

    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
    {
        XDefineCursor(bd->Display, bd->Window, None);
    } else
    {
        Cursor cursor = bd->MouseCursors[imgui_cursor] ? bd->MouseCursors[imgui_cursor] : bd->MouseCursors[ImGuiMouseCursor_Arrow];
        XDefineCursor(bd->Display, bd->Window, cursor);
    }
    bd->LastMouseCursor = imgui_cursor;
}

bool ImGui_ImplX11_Init(Display *display, Window window) {
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized!");

    ImGui_ImplX11_Data *bd = IM_NEW(ImGui_ImplX11_Data)();
    io.BackendPlatformUserData = (void *)bd;
    io.BackendPlatformName = "imgui_impl_x11";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

    bd->Display = display;
    bd->Window = window;

    // Create mouse cursors
    bd->MouseCursors[ImGuiMouseCursor_Arrow] = XCreateFontCursor(display, XC_left_ptr);
    bd->MouseCursors[ImGuiMouseCursor_TextInput] = XCreateFontCursor(display, XC_xterm);
    bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = XCreateFontCursor(display, XC_fleur);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = XCreateFontCursor(display, XC_sb_v_double_arrow);
    bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = XCreateFontCursor(display, XC_sb_h_double_arrow);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = XCreateFontCursor(display, XC_top_right_corner);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = XCreateFontCursor(display, XC_top_left_corner);
    bd->MouseCursors[ImGuiMouseCursor_Hand] = XCreateFontCursor(display, XC_hand2);
    bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = XCreateFontCursor(display, XC_circle);

    // Get initial display size
    XWindowAttributes wa;
    XGetWindowAttributes(display, window, &wa);
    io.DisplaySize = ImVec2((float)wa.width, (float)wa.height);

    return true;
}

void ImGui_ImplX11_Shutdown() {
    ImGui_ImplX11_Data *bd = ImGui_ImplX11_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown!");
    ImGuiIO &io = ImGui::GetIO();

    // Destroy cursors
    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
    {
        if (bd->MouseCursors[cursor_n])
            XFreeCursor(bd->Display, bd->MouseCursors[cursor_n]);
    }

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    IM_DELETE(bd);
}

void ImGui_ImplX11_NewFrame() {
    ImGui_ImplX11_Data *bd = ImGui_ImplX11_GetBackendData();
    IM_ASSERT(bd != nullptr && "Did you call ImGui_ImplX11_Init()?");
    ImGuiIO &io = ImGui::GetIO();

    // Update display size
    XWindowAttributes wa;
    XGetWindowAttributes(bd->Display, bd->Window, &wa);
    io.DisplaySize = ImVec2((float)wa.width, (float)wa.height);

    // Update time
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    unsigned long current_time = (unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
    io.DeltaTime = bd->Time > 0 ? (float)(current_time - bd->Time) / 1000.0f : (1.0f / 60.0f);
    bd->Time = current_time;

    // Update mouse cursor
    ImGui_ImplX11_UpdateMouseCursor();
}

bool ImGui_ImplX11_ProcessEvent(void *event_ptr) {
    XEvent *event = (XEvent *)event_ptr;
    ImGui_ImplX11_Data *bd = ImGui_ImplX11_GetBackendData();
    ImGuiIO &io = ImGui::GetIO();

    switch (event->type)
    {
    case ButtonPress:
    case ButtonRelease:
    {
        bool down = (event->type == ButtonPress);
        if (event->xbutton.button >= 1 && event->xbutton.button <= 5)
        {
            int button = event->xbutton.button - 1;
            if (button < 3)
            {
                io.AddMouseButtonEvent(button, down);
                bd->MouseButtonsDown[button] = down;
            } else if (button == 3) // Scroll up
            {
                if (down) io.AddMouseWheelEvent(0.0f, 1.0f);
            } else if (button == 4) // Scroll down
            {
                if (down) io.AddMouseWheelEvent(0.0f, -1.0f);
            }
        }
        return true;
    }
    case MotionNotify:
    {
        io.AddMousePosEvent((float)event->xmotion.x, (float)event->xmotion.y);
        return true;
    }
    case KeyPress:
    case KeyRelease:
    {
        KeySym keysym = XLookupKeysym(&event->xkey, 0);
        ImGuiKey key = ImGui_ImplX11_KeyToImGuiKey(keysym);
        bool down = (event->type == KeyPress);

        io.AddKeyEvent(key, down);

        // Handle modifiers
        io.AddKeyEvent(ImGuiMod_Ctrl, (event->xkey.state & ControlMask) != 0);
        io.AddKeyEvent(ImGuiMod_Shift, (event->xkey.state & ShiftMask) != 0);
        io.AddKeyEvent(ImGuiMod_Alt, (event->xkey.state & Mod1Mask) != 0);
        io.AddKeyEvent(ImGuiMod_Super, (event->xkey.state & Mod4Mask) != 0);

        // Handle text input
        if (event->type == KeyPress)
        {
            char buf[32];
            KeySym keysym_return;
            int count = XLookupString(&event->xkey, buf, sizeof(buf) - 1, &keysym_return, nullptr);
            if (count > 0)
            {
                buf[count] = '\0';
                io.AddInputCharactersUTF8(buf);
            }
        }
        return true;
    }
    case ConfigureNotify:
    {
        io.DisplaySize = ImVec2((float)event->xconfigure.width, (float)event->xconfigure.height);
        return true;
    }
    case FocusIn:
    case FocusOut:
    {
        io.AddFocusEvent(event->type == FocusIn);
        return true;
    }
    }

    return false;
}