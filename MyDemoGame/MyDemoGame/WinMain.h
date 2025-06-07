#pragma once
#include "../2DEngine/pch.h" 



class Winmain
{
public : 
	HWND g_hwnd = nullptr;
	HINSTANCE g_hInstance;

	UINT g_width = 1024;
	UINT g_height = 768;
	bool g_resized = false;

public : 
	virtual ~Winmain() = default; 
	void Run(); 
	virtual void Initialize();
	virtual void Uninitialize();
	void Render();	

};


