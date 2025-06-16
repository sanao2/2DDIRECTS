#include "pch.h"
#include "ImageManager.h"

void ImageManager::Initialize()
{
	HRESULT hr;
	ComPtr<IDXGIFactory7> dxgiFactory;
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"DXGI ���丮 ���� ����", L"����", MB_OK);
		return;
	}

	// SwapChain ����  
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = g_width;
	scDesc.Height = g_height;
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	dxgiFactory->CreateSwapChainForHwnd(
		g_d3dDevice.Get(), g_hwnd,
		&scDesc, nullptr, nullptr,
		g_dxgiSwapChain.GetAddressOf());

	if (FAILED(hr)) {
		MessageBox(nullptr, L"SwapChain ���� ����", L"����", MB_OK);
		return;
	}

	// ����۸� Ÿ������ ����  
	ComPtr<IDXGISurface> backBuffer;
	hr = g_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"����� �������� ����", L"����", MB_OK);
		return;
	}

	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED));

	hr = g_d2dDeviceContext->CreateBitmapFromDxgiSurface(
		backBuffer.Get(), &bmpProps,
		g_d2dBitmapTarget.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"��Ʈ�� ���� ����", L"����", MB_OK);
		return;
	}

	LoadImagePath(hr, L"../Resource/Mushroom.png");

}

void ImageManager::Render()
{
	BeginDrawClear(D2D1::ColorF::Black);	
	//DrawBitmapMatrix(g_d2dBitmapFromFile, obj);
	EndDraw();
}

// -------------------------- Draw ----------------------------------

void ImageManager::BeginDrawClear(const D2D1::ColorF color)
{
	g_d2dDeviceContext->BeginDraw();
	g_d2dDeviceContext->Clear(color);
}

void ImageManager::EndDraw()
{
	g_d2dDeviceContext->EndDraw();
	g_dxgiSwapChain->Present(1, 0); // ����۸� ���� ���۷� ������. ���. 
}



