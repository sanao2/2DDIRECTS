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

	// 원하는 크기로 조정되어 리턴  
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
		MessageBox(nullptr, L"WIC 팩토리 생성 실패", L"에러", MB_OK);
		return;
	}

	// D3D11 디바이스 생성  
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	hr = D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		levels, 1, D3D11_SDK_VERSION,
		g_d3dDevice.GetAddressOf(),
		&featureLevel, nullptr);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D3D11 디바이스 생성 실패", L"에러", MB_OK);
		return;
	}

	// D2D 팩토리 및 디바이스  
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		options, d2dFactory.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D2D 팩토리 생성 실패", L"에러", MB_OK);
		return;
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	g_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;

	hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D2D 디바이스 생성 실패", L"에러", MB_OK);
		return;
	}

	hr = d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		g_d2dDeviceContext.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D2D 디바이스 컨텍스트 생성 실패", L"에러", MB_OK);
		return;
	}

	__super::Initialize();

	/*D2D1_POINT_2F pos = { 10.0f,20.0f };
	D2D1_POINT_2F scale = { 10.0f, 10.0f }; 
	D2D1_POINT_2F rotate = { 5.0f, 5.0f }; 

	obj = new Object(pos, scale, rotate); */
}

void RenderManager::Uninitialize()
{
	Release();
}

void RenderManager::Render()
{
	__super::Render(); 
	//obj->ObjectRender(); 
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
