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
	g_dxgiSwapChain->Present(1, 0); // ����۸� ���� ���۷� ������. ���. 
}

void ImageManager::LoadImagePath(HRESULT hr, const wchar_t* path)
{
	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	hr = CreateBitmapFromFile(path,
		g_d2dBitmapFromFile.GetAddressOf());
}

HRESULT ImageManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap)
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

void ImageManager::DrawBitmapMatrix(ComPtr<ID2D1Bitmap1> pBitmap, Object* obj)
{
	//g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ 
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



