#pragma once
#include "pch.h"
#include "Object.h"
#include "Camera.h"

class ImageManager {
public : 
	virtual ~ImageManager() {}
	void Initialize(); 
	void Render(); 

	void BeginDrawClear(const D2D1::ColorF color);
	void EndDraw();
	void LoadImagePath(HRESULT hr, const wchar_t* path);
	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap);

	//test code 
	void DrawBitmapMatrix(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj);
	void DrawBitmapCamera(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj);

protected : 
	HWND g_hwnd = nullptr;
	UINT g_width = 1024;
	UINT g_height = 768;
	D2D1_SIZE_F bmpSize;

	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
	ComPtr<ID3D11Device>		g_d3dDevice;
	ComPtr<IDXGISwapChain1>		g_dxgiSwapChain;
	ComPtr<ID2D1Bitmap1>		g_d2dBitmapTarget;
	ComPtr<IWICImagingFactory>  g_wicImagingFactory;
	ComPtr<ID2D1Bitmap1>		g_d2dBitmapFromFile;

private : 
	Object* obj; 
	Camera* cam; 
};

