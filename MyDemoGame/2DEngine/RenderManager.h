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
	//PEG�� PNG, BMP, GIF �� ������ ���ڴ�(IWICBitmapDecoder)�� ���������� �����ϰ�
	//�ش� ������ �ؼ�(���ڵ�)�Ͽ� �޸𸮻��� ��Ʈ�� �������� ���� �� �ְ� ��. 

};

