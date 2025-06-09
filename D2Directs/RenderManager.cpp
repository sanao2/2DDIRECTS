#include "pch.h"
#include "RenderManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
void RenderManager::Initialize()
{

	HRESULT hr;

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = g_hInstance;
	wc.lpszClassName = L"MyD2DWindowClass";
	RegisterClass(&wc);

	// ���ϴ� ũ��� �����Ǿ� ����  
	SIZE  clientSize = { (LONG)g_width, (LONG)g_height };
	RECT  clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	g_hwnd = CreateWindowEx(
		0, L"MyD2DWindowClass", L"D2D1 Imagine Example",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,
		nullptr, nullptr, g_hInstance, this);
	ShowWindow(g_hwnd, SW_SHOW);
	UpdateWindow(g_hwnd);

	//// Create WIC factory  
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(g_wicImagingFactory),
		(void**)g_wicImagingFactory.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"WIC ���丮 ���� ����", L"����", MB_OK);
		return;
	}

	// D3D11 ����̽� ����  
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	hr = D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		levels, 1, D3D11_SDK_VERSION,
		g_d3dDevice.GetAddressOf(),
		&featureLevel, nullptr);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D3D11 ����̽� ���� ����", L"����", MB_OK);
		return;
	}

	// D2D ���丮 �� ����̽�  
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		options, d2dFactory.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D2D ���丮 ���� ����", L"����", MB_OK);
		return;
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	g_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;

	hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D2D ����̽� ���� ����", L"����", MB_OK);
		return;
	}

	hr = d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		g_d2dDeviceContext.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D2D ����̽� ���ؽ�Ʈ ���� ����", L"����", MB_OK);
		return;
	}

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

	hr = dxgiFactory->CreateSwapChainForHwnd(
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

void RenderManager::LoadImagePath(HRESULT hr, const wchar_t* path)
{
	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	hr = CreateBitmapFromFile(path,
		g_d2dBitmapFromFile.GetAddressOf());
}


HRESULT RenderManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;    // �̹��� ���� ���ڴ�. ��Ʈ�� �����͸� ���ڵ��Ͽ� ������ ������ ��Ʈ�� ��ü�� ��ȯ. ��, �ȼ� ������ ����.  
	ComPtr<IWICBitmapFrameDecode> frame;	  // �̹����� �׸� ������ 
	ComPtr<IWICFormatConverter>   converter;  // WIC(Windows Imaging Component)���� �о���� ��Ʈ�� �ҽ��� �ٸ� �ȼ� �������� ��ȯ

	//  ���ڴ� ����
	HRESULT hr = g_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// ù ������ ���
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// ���� ��ȯ�� ����
	hr = g_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// GUID_WICPixelFormat32bppPBGRA�� ��ȯ
	hr = converter->Initialize(
		frame.Get(),					// � ��Ʈ��(������)�� ��ȯ�� ���ΰ�?���� �����ϴ� ����
		GUID_WICPixelFormat32bppPBGRA,  // � �ȼ� �������� ��ȯ�� ���������� �����ϴ� GUID
		WICBitmapDitherTypeNone,        // �ȼ� �� ��踦 �ε巴�� ǥ�� ���� ���� // ������ ���ϴ� ��. 
		nullptr,						// � ���� �ȷ�Ʈ ��� ������ ����� ������ ����. nullptr�̸� �ȷ�Ʈ ���� 
		0.0f,							// ���� �Ӱ谪
		WICBitmapPaletteTypeCustom	    // ���� �̹������� �ȷ�Ʈ ��� ��ȯ ��, � �ȷ�Ʈ ������ �� ���������� ���� // ���� �ȷ�Ʈ�� ���� �ȷ�Ʈ ������ ���� 
	);
	// HRESULT�� ��ȯ�մϴ�. �����ϸ� S_OK �Ǵ� S_FALSE, �����ϸ� ������ ���� �ڵ�(E_FAIL, E_INVALIDARG ��)
	if (FAILED(hr)) return hr;

	// Direct2D ��Ʈ�� �Ӽ� (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,// ��Ʈ�� �ɼ�. GDI �� �Բ� �� ������ D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE ��� 
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// DeviceContext���� WIC ��Ʈ�����κ��� D2D1Bitmap1 ����
	hr = g_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, Bitmap);
	return hr;

}
void RenderManager::Uninitialize()
{
	g_d3dDevice = nullptr;
	g_dxgiSwapChain = nullptr;
	g_d2dBitmapTarget = nullptr;
	g_wicImagingFactory = nullptr;
	g_d2dDeviceContext = nullptr;
	g_d2dBitmapFromFile = nullptr;
	Release();
}

void RenderManager::Render()
{
	BeginDrawClear(D2D1::ColorF::White);
	DrawBitmap(g_d2dBitmapFromFile, D2D1::RectF(0, 3, 30.0f, 20.0f));
	EndDraw();
}
void RenderManager::Release()
{
	g_d2dDeviceContext = nullptr;
	g_d3dDevice = nullptr;
	g_dxgiSwapChain = nullptr;
	g_d2dBitmapTarget = nullptr;
	g_wicImagingFactory = nullptr;
	g_d2dBitmapFromFile = nullptr;
}

void RenderManager::BeginDrawClear(const D2D1::ColorF color)
{
	g_d2dDeviceContext->BeginDraw();
	g_d2dDeviceContext->Clear(color);
}

void RenderManager::DrawBitmap(ComPtr<ID2D1Bitmap1> pBitmap, const D2D1_RECT_F destrect) const
{
	g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ 
	g_d2dDeviceContext->DrawBitmap(pBitmap.Get(), destrect);
}

void RenderManager::EndDraw()
{
	g_d2dDeviceContext->EndDraw();
	g_dxgiSwapChain->Present(1, 0); // ����۸� ���� ���۷� ������. ���. 
}