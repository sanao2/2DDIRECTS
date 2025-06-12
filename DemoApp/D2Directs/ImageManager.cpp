#include "pch.h"
#include "ImageManager.h"

void ImageManager::Initialize()
{
	HRESULT hr;
	ComPtr<IDXGIFactory7> dxgiFactory;
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"DXGI 팩토리 생성 실패", L"에러", MB_OK);
		return;
	}

	// SwapChain 생성  
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
		MessageBox(nullptr, L"SwapChain 생성 실패", L"에러", MB_OK);
		return;
	}

	// 백버퍼를 타겟으로 설정  
	ComPtr<IDXGISurface> backBuffer;
	hr = g_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"백버퍼 가져오기 실패", L"에러", MB_OK);
		return;
	}

	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED));

	hr = g_d2dDeviceContext->CreateBitmapFromDxgiSurface(
		backBuffer.Get(), &bmpProps,
		g_d2dBitmapTarget.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"비트맵 생성 실패", L"에러", MB_OK);
		return;
	}

	LoadImagePath(hr, L"../Resource/Mushroom.png");

	// Camera Test 
	D2D1_POINT_2F camPos = { 0, 0 }; 
	D2D1_POINT_2F camScale = { 1.0f, 1.0f }; 
	float camAngle = 0.0f; 

	cam = new Camera(camPos, camScale, camAngle); 
	cam->Initialize(); 

	// Object Test 
	D2D1_POINT_2F pos = { 70.0f, 50.0f };
	bmpSize = g_d2dBitmapFromFile->GetSize();
	D2D1_POINT_2F scale = { bmpSize.width, bmpSize.height };

	obj = new Object(pos, scale, 5.0f);
	obj->SetRenderTarget(g_d2dDeviceContext);
	obj->Initialize();


}

void ImageManager::Render()
{
	BeginDrawClear(D2D1::ColorF::Black);
	//DrawBitmapObj(g_d2dBitmapFromFile, obj);
	//DrawBitmapMatrix(g_d2dBitmapFromFile, obj);
	DrawBitmapCamera(g_d2dBitmapFromFile, obj);
	EndDraw();
}

void ImageManager::BeginDrawClear(const D2D1::ColorF color)
{
	g_d2dDeviceContext->BeginDraw();
	g_d2dDeviceContext->Clear(color);
}

void ImageManager::EndDraw()
{
	g_d2dDeviceContext->EndDraw();
	g_dxgiSwapChain->Present(1, 0); // 백버퍼를 전면 버퍼로 스와핑. 출력. 
}

void ImageManager::LoadImagePath(HRESULT hr, const wchar_t* path)
{
	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	hr = CreateBitmapFromFile(path,
		g_d2dBitmapFromFile.GetAddressOf());
}

HRESULT ImageManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap)
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

	// Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,// 비트맵 옵션. GDI 와 함께 쓸 때에는 D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE 사용 
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = g_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, Bitmap);
	return hr;

}

void ImageManager::DrawBitmapMatrix(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj)
{
	//g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화 
	//obj->Objrect = { obj->transforms.Pos.x, obj->transforms.Pos.y, obj->transforms.Scale.x , obj->transforms.Scale.y };

	obj->UpdateTransformMatrix();
	g_d2dDeviceContext->SetTransform(obj->transform);

	float width = (obj->Objrect.right - obj->Objrect.left) * 2;
	float height = (obj->Objrect.bottom - obj->Objrect.top) / 2;

	D2D1_RECT_F drawRect = D2D1::RectF(g_width / 2, g_height / 2, width, height);

	g_d2dDeviceContext->DrawBitmap(pBitmap.Get(), drawRect);

}

void ImageManager::DrawBitmapCamera(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj)
{
	obj->UpdateTransformMatrix();

	D2D1_MATRIX_3X2_F CameraMatrix = cam->GetCameraMatrix() * obj->transform;
	g_d2dDeviceContext->SetTransform(CameraMatrix);

	D2D1_RECT_F drawRect = D2D1::RectF(0, 0, bmpSize.width, bmpSize.height);

	g_d2dDeviceContext->DrawBitmap(pBitmap.Get(), drawRect);

}



