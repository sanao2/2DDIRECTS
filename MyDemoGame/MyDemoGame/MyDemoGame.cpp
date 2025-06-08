#include "MyDemoGame.h"

// 전역 변수
// UnInitlialize() 함수 호출 이후 해제해야함 
// Relese() 함수 호출 

HWND g_hwnd = nullptr;
HINSTANCE g_hInstance;

UINT g_width = 1024;
UINT g_height = 768;
bool g_resized = false;
// For ImageDraw
//PEG나 PNG, BMP, GIF 등 적절한 디코더(IWICBitmapDecoder)를 내부적으로 선택하고
//해당 파일을 해석(디코딩)하여 메모리상의 비트맵 프레임을 얻을 수 있게 함. 
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			break; // 최소화는 무시  

		UINT width = LOWORD(lParam); // 새 너비  
		UINT height = HIWORD(lParam); // 새 높이  

		// 특정 Winmain 객체를 참조하도록 수정  
		if (g_width != width || g_height != height)
		{
			g_width = width;
			g_height = height;
			g_resized = true;
		}
	}
	break;
	case WM_EXITSIZEMOVE:
		if (g_resized)
		{
			//apps.Uninitialize();
			//apps.Initialize();
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HRESULT Application::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;    // 이미지 포멧 디코더. 스트림 데이터를 디코딩하여 프레임 단위의 비트맵 객체로 변환. 즉, 픽셀 정보를 추출.  
	ComPtr<IWICBitmapFrameDecode> frame;	  // 이미지를 그릴 프레임 
	ComPtr<IWICFormatConverter>   converter;  // WIC(Windows Imaging Component)에서 읽어들인 비트맵 소스를 다른 픽셀 포맷으로 변환

	//  디코더 생성
	HRESULT hr = g_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// 첫 프레임 얻기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// 포맷 변환기 생성
	hr = g_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// GUID_WICPixelFormat32bppPBGRA로 변환
	hr = converter->Initialize(
		frame.Get(),					// 어떤 비트맵(프레임)을 변환할 것인가?”를 지정하는 역할
		GUID_WICPixelFormat32bppPBGRA,  // 어떤 픽셀 포맷으로 변환할 것인지”를 지정하는 GUID
		WICBitmapDitherTypeNone,        // 픽셀 간 경계를 부드럽게 표현 여부 결정 // 블렌딩을 말하는 것. 
		nullptr,						// 어떤 색상 팔레트 기반 포맷을 사용할 것인지 지정. nullptr이면 팔레트 없음 
		0.0f,							// 알파 임계값
		WICBitmapPaletteTypeCustom	    // 원본 이미지에서 팔레트 기반 변환 시, 어떤 팔레트 전략을 쓸 것인지”를 지정 // 고정 팔레트와 동적 팔레트 전략이 존재 
	);
	// HRESULT를 반환합니다. 성공하면 S_OK 또는 S_FALSE, 실패하면 적절한 에러 코드(E_FAIL, E_INVALIDARG 등)
	if (FAILED(hr)) return hr;

	// ⑤ Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,// 비트맵 옵션. GDI 와 함께 쓸 때에는 D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE 사용 
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = g_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	return hr;
	
}
void Application::LoadeImageFromFile(const wchar_t* path)
{
	//이미지 파일(PNG 등)을 로드하여 ID2D1Bitmap1 객체로 만드는 초기화 과정
	CreateBitmapFromFile(path, g_d2dBitmapFromFile.GetAddressOf());
}

void Application::Run()
{
	// 메시지 루프
	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Render();
		}
	}
}


void Application::Initialize()
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = g_hInstance;
	wc.lpszClassName = L"MyD2DWindowClass";
	RegisterClass(&wc);
	SIZE clientSize = { (LONG)g_width,(LONG)g_height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	g_hwnd = CreateWindowEx(0, L"MyD2DWindowClass", L"D2D1 Imagine Example",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
		nullptr, nullptr, g_hInstance, nullptr);	

	CoInitialize(nullptr);

	//// Create WIC factory		
	CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(g_wicImagingFactory),
		(void**)g_wicImagingFactory.GetAddressOf());
	/*CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory),
		(void**)g_wicImagingFactory.GetAddressOf());*/
	LoadeImageFromFile(L"../Resource/Mushroom.png");
}

void Application::Uninitialize()
{
	g_wicImagingFactory = nullptr;
	g_d2dBitmapFromFile = nullptr;
	g_d2dDeviceContext = nullptr;

}

void Application::Render()
{
}

