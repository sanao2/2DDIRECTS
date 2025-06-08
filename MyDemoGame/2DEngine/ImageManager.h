#pragma once
#include "pch.h"
#include "RenderManager.h"

class ImageManager : public RenderManager 
{
private :
	HWND hwnd = nullptr;
	UINT width;
	UINT height;
	ComPtr<ID3D11Device> g_d3dDevice;
	ComPtr<IDXGISwapChain1> g_dxgiSwapChain;
	ComPtr<ID2D1Bitmap1> g_d2dBitmapTarget;
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
public : 
	ImageManager(HWND hwnd, UINT width, UINT height, ComPtr<ID2D1DeviceContext7> d2dDeviceContext)
		: hwnd(hwnd), width(width), height(height), g_d2dDeviceContext(d2dDeviceContext)
	{
	}

	void Initialize() override;
	void Uninitialize() override;
	void Render() override;

	void BeginDrawClear(const D2D1::ColorF color);
	void DrawBitmap(ComPtr<ID2D1Bitmap1> pBitmap, const D2D1_RECT_F destrect) const;
	void EndDraw();
	
};

