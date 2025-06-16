#pragma once
#include "pch.h"
#include "Camera.h"
#include "Image.h"

class ImageManager : public Image
{
public:
	virtual ~ImageManager() {}
	void Initialize();
	void Render();

	void BeginDrawClear(const D2D1::ColorF color);
	void EndDraw();
	
protected:
	HWND g_hwnd = nullptr;
	UINT g_width = 1024;
	UINT g_height = 768;
	D2D1_SIZE_F bmpSize;

	ComPtr<ID3D11Device>		g_d3dDevice;
	ComPtr<IDXGISwapChain1>		g_dxgiSwapChain;	

};

