#pragma once

enum Bitmap
{
	MUSHROOM,
	BLOOD
};

class Image
{
private:
	// 리소스를 불러올 경로 변수 
	wchar_t* path;
	Bitmap bitmapKey;

protected:
	ComPtr<IWICImagingFactory>  g_wicImagingFactory;
	ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1>		g_d2dBitmapTarget;
	ComPtr<ID2D1Bitmap1>		g_d2dBitmapFromFile;

	void LoadImagePath(HRESULT hr, const wchar_t* path);
	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap);

public:
	ComPtr<ID2D1Bitmap1> GetBitmap() { return g_d2dBitmapFromFile.Get(); }
};

