#include "RenderManager.h"
// WIC를 통해 PNG 등을 로드하여 ID2D1Bitmap1**으로 반환
void RenderManager::Initialize()
{
	// D3D11 디바이스 생성
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, g_d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D 팩토리 및 디바이스
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	g_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;

	d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, g_d2dDeviceContext.GetAddressOf());

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain 생성 // 더블 버퍼링. 두개의 버퍼를 체인처럼 연결해서 사용할 수 있게 하는 것. 
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;								  // 버퍼 너비. 가로 길이 
	scDesc.Height = height;								  // 버퍼 높이. 세로 길이 
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			  // 각 픽셀 포맷 
	scDesc.SampleDesc.Count = 1;						  // 멀티 샘플링 사용 여부 (1 =  사용안함)  멀티 샘플링 : 가장자리가 부드러워지느냐.   
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 그릴 버퍼로 사용한다는 명시적 코드 
	scDesc.BufferCount = 2;								  // 더블 버퍼링을 위해 2개의 버퍼를 사용 
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	  // 버퍼를 교체하는 방식 
	dxgiFactory->CreateSwapChainForHwnd(g_d3dDevice.Get(), hwnd, &scDesc, nullptr, nullptr, g_dxgiSwapChain.GetAddressOf()); // 특정 윈도우 창에 대해서 스왑 체인을 생성 하는 것 


	// 백버퍼를 타겟으로 설정
	ComPtr<IDXGISurface> backBuffer;
	g_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)); // 사용할 백버퍼를 스왑 체인에서 IDXGISurface 타입으로 가져온다. 

	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	// “어떤 픽셀 포맷으로, 어떤 옵션을 가진 비트맵”인지 알려주기 위해, Direct2D의 비트맵 속성을 설정하고, 구조체 D2D1_BITMAP_PROPERTIES1 에 담는다. 


	g_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, g_d2dBitmapTarget.GetAddressOf()); // “IDXGISurface를 바로 Direct2D의 ID2D1Bitmap1 객체로 변환(Wrap)해 달라”는 명령
	// 직접 GPU 메모리의 텍스처를 복사하는 과정 없이, 동일한 메모리를 공용(shared)으로 사용한다. 

	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	//Direct2D의 장치 컨텍스트(ID2D1DeviceContext)가 “이제부터 그릴 대상(target)을 방금 래핑한 비트맵으로 삼겠다”
	LoadeImageFromFile(L"../Resource/Mushroom.png");

}
void RenderManager::Uninitialize()
{
	g_d3dDevice = nullptr;
	g_dxgiSwapChain = nullptr;
	g_d2dDeviceContext = nullptr;
	g_d2dBitmapTarget = nullptr;
}
void RenderManager::Render() 
{
	BeginDrawClear(D2D1::ColorF::Black); // 검은색으로 화면을 초기화 

	DrawBitmap(g_d2dBitmapTarget, D2D1::RectF(0, 0, (float)width, (float)height));

	EndDraw();
}

void RenderManager::BeginDrawClear(const D2D1::ColorF color)
{
	g_d2dDeviceContext->BeginDraw();
	g_d2dDeviceContext->Clear(color);
}

void RenderManager::DrawBitmap(ComPtr<ID2D1Bitmap1> Bitmap, const D2D1_RECT_F destrect) const
{
	g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화 
	g_d2dDeviceContext->DrawBitmap(Bitmap.Get(), destrect);
}

void RenderManager::EndDraw()
{
	g_d2dDeviceContext->EndDraw();
	g_dxgiSwapChain->Present(1, 0); // 백버퍼를 전면 버퍼로 스와핑. 출력. 
}
// 스왑 체인에 있는 백버퍼를 전면 버퍼로 교체하는 작업. 