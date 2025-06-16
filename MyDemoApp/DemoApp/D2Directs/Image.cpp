#include "pch.h"
#include "Image.h"

//void Image::SetImagePath()
//{
//	wchar_t resourcePath[] = { L"Mushroom.png" , L"Blood.png" };
//}

// ---------------------------------- Load Image -----------------------------

void Image::LoadImagePath(HRESULT hr, const wchar_t* path)
{
	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	hr = CreateBitmapFromFile(path,
		g_d2dBitmapFromFile.GetAddressOf());
}

HRESULT Image::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** Bitmap)
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
