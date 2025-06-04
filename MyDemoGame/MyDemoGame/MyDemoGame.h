#pragma once
#include "../2DEngine/pch.h" 
#include "resource.h"

class Application
{
private : 
	HWND g_hwnd = nullptr;

protected : 
	ComPtr<ID3D11Device> g_d3dDevice;
	ComPtr<IDXGISwapChain1> g_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> g_d2dBitmapTarget;

public : 
	void Initialize(HWND hwnd); 
	void Uninitialize();
	void Render();

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow);
};