#pragma once
#include "../2DEngine/pch.h" 

extern HWND g_hwnd ;
extern HINSTANCE g_hInstance;
extern UINT g_width ;
extern UINT g_height;

class Winmain
{
public : 
	virtual ~Winmain() = default; 
	void Run(); 
	void Initialize();
	void Uninitialize();
	void Render();	

};


