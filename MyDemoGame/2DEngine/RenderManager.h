#pragma once
#include "pch.h"
#include "../MyDemoGame/MyDemoGame.h"

class RenderManager : public Application 
{
public : 
	void Initialize(HWND hwnd, UINT width, UINT height) override; 
	void Render() override; 
	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
	void Uninitialize();  

private : 
	ComPtr<IWICImagingFactory> g_wicImagingFactory;
	// For ImageDraw
	//PEG나 PNG, BMP, GIF 등 적절한 디코더(IWICBitmapDecoder)를 내부적으로 선택하고
	//해당 파일을 해석(디코딩)하여 메모리상의 비트맵 프레임을 얻을 수 있게 함. 

};

