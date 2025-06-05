#pragma once
#include "../2DEngine/pch.h" 
#include "MyDemoGame.h"


HWND g_hwnd = nullptr;
HINSTANCE g_hInstance; 

UINT g_width = 1024;
UINT g_height = 768;
bool g_resized = false;

class Winmain
{
public : 
	void Run(); 
	virtual void Initialize();
	void Uninitialize(); 
	virtual void Render();	

};

