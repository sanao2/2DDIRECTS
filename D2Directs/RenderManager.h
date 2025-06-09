#pragma once
#include <exception>

class RenderManager 
{
public : 
	void Initialize() ; 
	void Uninitialize() ;
	void Render();
	void Release(); 
	
	void LoadImagePath(HRESULT hr, const wchar_t* path);
	void BeginDrawClear(const D2D1::ColorF color);
	void DrawBitmap(ComPtr<ID2D1Bitmap1> pBitmap, const D2D1_RECT_F destrect) const;
	void EndDraw();
	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap);

private:
	HWND g_hwnd = nullptr;
	UINT g_width = 1024;
	UINT g_height = 768;
	bool g_resized = false;

	HINSTANCE					g_hInstance = nullptr; // 윈도우 인스턴스 핸들 
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
	ComPtr<ID3D11Device>		g_d3dDevice;
	ComPtr<IDXGISwapChain1>		g_dxgiSwapChain;
	ComPtr<ID2D1Bitmap1>		g_d2dBitmapTarget;
	ComPtr<IWICImagingFactory>  g_wicImagingFactory;
	ComPtr<ID2D1Bitmap1>		g_d2dBitmapFromFile;


};

