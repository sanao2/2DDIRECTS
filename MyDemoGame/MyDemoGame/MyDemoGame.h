#pragma once
#include "resource.h"
#include "../2DEngine/pch.h" 

UINT g_width = 1024;
UINT g_height = 768;
bool g_resized = false;

class Application
{
private:
	HWND g_hwnd = nullptr;

protected:
	ComPtr<ID3D11Device> g_d3dDevice;
	ComPtr<IDXGISwapChain1> g_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> g_d2dBitmapTarget;
	ComPtr<ID2D1Bitmap1> g_d2dBitmapFromFile; // WIC를 통해 PNG 등을 로드하여 ID2D1Bitmap1**으로 반환

public:
	virtual void Initialize(HWND hwnd, UINT width, UINT height);
	virtual void Render();
	void Uninitialize();
	void Run();
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow);

};