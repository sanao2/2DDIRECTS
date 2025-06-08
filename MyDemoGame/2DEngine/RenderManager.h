#include "pch.h"
#pragma once
#include "ImageManager.h"
#include <iostream> 
using namespace std; 

class RenderManager 
{
private : 
	HWND hwnd = nullptr;
	UINT width;
	UINT height;
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
public : 
	RenderManager(HWND hwnd, UINT width, UINT height, ComPtr<ID2D1DeviceContext7> d2dDeviceContext)
		: hwnd(hwnd) , width(width) , height(height), g_d2dDeviceContext(d2dDeviceContext)
	{}
	virtual ~RenderManager() {}

	virtual void Initialize();
	virtual void Uninitialize();
	virtual void Render();

	ComPtr<ID2D1DeviceContext7> getd2dContext() { return g_d2dDeviceContext; }
};

