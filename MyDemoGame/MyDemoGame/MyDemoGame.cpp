#include "MyDemoGame.h"

// 전역 변수
// UnInitlialize() 함수 호출 이후 해제해야함 
// Relese() 함수 호출 


// For ImageDraw
//PEG나 PNG, BMP, GIF 등 적절한 디코더(IWICBitmapDecoder)를 내부적으로 선택하고
//해당 파일을 해석(디코딩)하여 메모리상의 비트맵 프레임을 얻을 수 있게 함. 

void Application::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;    // 이미지 포멧 디코더. 스트림 데이터를 디코딩하여 프레임 단위의 비트맵 객체로 변환. 즉, 픽셀 정보를 추출.  
	ComPtr<IWICBitmapFrameDecode> frame;	  // 이미지를 그릴 프레임 
	ComPtr<IWICFormatConverter>   converter;  // WIC(Windows Imaging Component)에서 읽어들인 비트맵 소스를 다른 픽셀 포맷으로 변환

	// 디코더 생성
	g_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	
	// 첫 프레임 얻기
	decoder->GetFrame(0, &frame);
	
	// 포맷 변환기 생성
	g_wicImagingFactory->CreateFormatConverter(&converter);
	
	// GUID_WICPixelFormat32bppPBGRA로 변환
	converter->Initialize(
		frame.Get(),					// 어떤 비트맵(프레임)을 변환할 것인가?”를 지정하는 역할
		GUID_WICPixelFormat32bppPBGRA,  // 어떤 픽셀 포맷으로 변환할 것인지”를 지정하는 GUID
		WICBitmapDitherTypeNone,        // 픽셀 간 경계를 부드럽게 표현 여부 결정 // 블렌딩을 말하는 것. 
		nullptr,						// 어떤 색상 팔레트 기반 포맷을 사용할 것인지 지정. nullptr이면 팔레트 없음 
		0.0f,							// 알파 임계값
		WICBitmapPaletteTypeCustom	    // 원본 이미지에서 팔레트 기반 변환 시, 어떤 팔레트 전략을 쓸 것인지”를 지정 // 고정 팔레트와 동적 팔레트 전략이 존재 
	);
	// HRESULT를 반환합니다. 성공하면 S_OK 또는 S_FALSE, 실패하면 적절한 에러 코드(E_FAIL, E_INVALIDARG 등)
	

	// Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,// 비트맵 옵션. GDI 와 함께 쓸 때에는 D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE 사용 
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	//g_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	pRManager->getd2dContext()->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap); 
	
}

void Application::Initialize()
{
	//// Create WIC factory
	CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(g_wicImagingFactory),
		(void**)g_wicImagingFactory.GetAddressOf());

	//이미지 파일(PNG 등)을 로드하여 ID2D1Bitmap1 객체로 만드는 초기화 과정
	CreateBitmapFromFile(L"../Resource/mushroom.png", g_d2dBitmapFromFile.GetAddressOf());
	//assert(SUCCEEDED(hr));
}

void Application::Uninitialize()
{
	g_wicImagingFactory = nullptr;
	g_d2dBitmapFromFile = nullptr;
	g_d2dDeviceContext = nullptr;
}

void Application::Render()
{}

