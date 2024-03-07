#ifndef DRAWING_H
#define DRAWING_H

#include <Windows.h>
#include <tchar.h>
#include "d3d9.h"
#include "d3d11.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;

public:
	static void Active();
	static bool isActive();
	static void Draw();
};

#endif
