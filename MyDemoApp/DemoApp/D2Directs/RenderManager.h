#pragma once
#include "ImageManager.h"

class RenderManager : public ImageManager
{
public:
	RenderManager() = default;
	void Render();
	void Initialize();
	void Uninitialize();
	void Release();
	D2D1::Matrix3x2F GetMatrix();
	//test code 
	void DrawBitmapTransform(ComPtr<ID2D1Bitmap1>& Bitmap, const D2D1_RECT_F* offset = nullptr, const D2D1_MATRIX_3X2_F& transform,
		D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		float opacity = 1.0f);

private:
	bool g_resized = false;
	HINSTANCE  g_hInstance = nullptr; // 윈도우 인스턴스 핸들 
	D2D1::Matrix3x2F m_renderMatrix; 

};

