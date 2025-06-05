#pragma once
#include "../MyDemoGame/Winmain.h"

HWND g_hwnd = nullptr;
HINSTANCE g_hInstance;

UINT g_width = 1024;
UINT g_height = 768;
bool g_resized = false;

class WInApplication 
{
public:
	virtual ~WInApplication() = default;
	void Run();
	
	void Initialize();
	void Uninitialize();
	void Render();
};

