// generated with claude.ai

#pragma once 
#include "imgui.h"

struct _XDisplay;
typedef struct _XDisplay Display;
typedef unsigned long Window;
typedef unsigned long XID;
typedef XID Cursor;

IMGUI_IMPL_API bool     ImGui_ImplX11_Init(Display *display, Window window);
IMGUI_IMPL_API void     ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplX11_NewFrame();
IMGUI_IMPL_API bool     ImGui_ImplX11_ProcessEvent(void *event);