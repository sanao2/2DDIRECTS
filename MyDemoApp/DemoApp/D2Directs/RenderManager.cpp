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


}
void RenderManager::Render()
{
	BeginDrawClear(D2D1::ColorF::Black);
	EndDraw();
}
void RenderManager::Uninitialize()
{
	Release();
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

D2D1::Matrix3x2F RenderManager::GetMatrix()
{
	return m_renderMatrix;
}

/*void RenderManager::DrawBitmapMatrix(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj)
{
	g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화 

	D2D1_VECTOR_2F pos = { 0.0f, 0.0f }; 
	D2D1_VECTOR_2F scale = { 1.0f, 1.0f }; 


	//g_d2dDeviceContext->SetTransform(obj->m_transform->Matrix());

	g_d2dDeviceContext->DrawBitmap(pBitmap.Get(), obj->LocalRect);

}

 void RenderManager::DrawBitmap(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj)
{
	g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화
	g_d2dDeviceContext->DrawBitmap(pBitmap.Get(), obj->LocalRect);
} */

void RenderManager::DrawBitmapTransform(ComPtr<ID2D1Bitmap1>& Bitmap, const D2D1_RECT_F* offset = nullptr, 
	const D2D1_MATRIX_3X2_F& transform, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode, float opacity )
{
	// 부모 와 자식들 각각의 트랜스폼에 대한 변환이 있을 때, 기존에 있는 트랜스폼을 기반으로 해서 출력
	g_d2dDeviceContext->SetTransform(transform); // 변환 초기화
	g_d2dDeviceContext->DrawBitmap(Bitmap.Get(), offset, opacity, interpolationMode);
}



