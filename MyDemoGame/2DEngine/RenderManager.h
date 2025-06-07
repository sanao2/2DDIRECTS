#pragma once
#include "pch.h"
#include "../MyDemoGame/WinMain.h"

class RenderManager : public Winmain
{
private : 
	HWND hwnd;
	UINT width;
	UINT height;
	ComPtr<ID3D11Device> g_d3dDevice;
	ComPtr<IDXGISwapChain1> g_dxgiSwapChain;
	ComPtr<ID2D1Bitmap1> g_d2dBitmapTarget;
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
public : 
	RenderManager(HWND hwnd, UINT width, UINT height) : hwnd(hwnd), width(width), height(height)
	{}
	void Initialize() override;
	void Uninitialize(); 
	void Render() override; 

	void BeginDrawClear(const D2D1::ColorF color); 
	void DrawBitmap(ComPtr<ID2D1Bitmap1> pBitmap, const D2D1_RECT_F destrect) const;
	void EndDraw(); 
	ComPtr<ID2D1DeviceContext7> getd2dContext() { return g_d2dDeviceContext; } 
};

