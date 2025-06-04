#include "RenderManager.h"


HRESULT RenderManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;    // �̹��� ���� ���ڴ�. ��Ʈ�� �����͸� ���ڵ��Ͽ� ������ ������ ��Ʈ�� ��ü�� ��ȯ. ��, �ȼ� ������ ����.  
	ComPtr<IWICBitmapFrameDecode> frame;	  // �̹����� �׸� ������ 
	ComPtr<IWICFormatConverter>   converter;  // WIC(Windows Imaging Component)���� �о���� ��Ʈ�� �ҽ��� �ٸ� �ȼ� �������� ��ȯ

	// �� ���ڴ� ����
	HRESULT hr = g_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// �� ù ������ ���
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// �� ���� ��ȯ�� ����
	hr = g_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// �� GUID_WICPixelFormat32bppPBGRA�� ��ȯ
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

	// �� Direct2D ��Ʈ�� �Ӽ� (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,// ��Ʈ�� �ɼ�. GDI �� �Բ� �� ������ D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE ��� 
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// �� DeviceContext���� WIC ��Ʈ�����κ��� D2D1Bitmap1 ����
	hr = g_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	return hr;
}

void RenderManager::Initialize(HWND hwnd, UINT width, UINT height)
{
	HRESULT hr;
	// D3D11 ����̽� ����
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, g_d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D ���丮 �� ����̽�
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	g_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;

	d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, g_d2dDeviceContext.GetAddressOf());

	/// <summary>
	/// ���⼭���Ͱ� SwapChain Ŭ�������� �������� �� �κ� 
	/// </summary>
	/// <param name="hwnd"></param>
	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));


	// SwapChain ���� // ���� ���۸�. �ΰ��� ���۸� ü��ó�� �����ؼ� ����� �� �ְ� �ϴ� ��. 
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = g_width;								  // ���� �ʺ�. ���� ���� 
	scDesc.Height = g_height;							  // ���� ����. ���� ���� 
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			  // �� �ȼ� ���� 
	scDesc.SampleDesc.Count = 1;						  // ��Ƽ ���ø� ��� ���� (1 =  ������)  ��Ƽ ���ø� : �����ڸ��� �ε巯��������.   
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �׸� ���۷� ����Ѵٴ� ����� �ڵ� 
	scDesc.BufferCount = 2;								  // ���� ���۸��� ���� 2���� ���۸� ��� 
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	  // ���۸� ��ü�ϴ� ��� 
	dxgiFactory->CreateSwapChainForHwnd(g_d3dDevice.Get(), hwnd, &scDesc, nullptr, nullptr, g_dxgiSwapChain.GetAddressOf()); // Ư�� ������ â�� ���ؼ� ���� ü���� ���� �ϴ� �� 
	// ��, ��� ������ â�� �ִ� ����̽��� ������ �� ���ϰ�, 
	// ���� ũ��� ���� ������ �о�ͼ�, 
	// ��ũ�� ���(Ǯ ��ũ�� Ȥ�� ������ ��ũ�� ���)�� ������, 
	// ����� ȭ���� ���ؼ� 
	// ������ ���� ü���� ��� ������� �� �ְ� �ϴ� ��.


	// ����۸� Ÿ������ ����
	ComPtr<IDXGISurface> backBuffer;
	g_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)); // ����� ����۸� ���� ü�ο��� IDXGISurface Ÿ������ �����´�. 
	// IDXGISurface�� ��DXGI ǥ�� Surface �������̽�����, GPU ���� �ؽ�ó(�Ǵ� ���� Ÿ��)�� COM �������̽� ���·� �ٷ� �� ���
	// Direct2D�� �ٷ� �� ���� �׸� �� �ֵ��� ����Ǵ� �ٸ� ������ �Ѵ�. 
	// ��, ����۸� �����ͼ� Direct2D�� �̹����� �׷��� �� �ִ� Surface �� ��ȯ �ϴ� ��. 
/// <summary>
/// ��������� SwapChin Ŭ�������� �������� �� �κ� 
/// </summary>

/// <summary>
/// ���⼭���Ͱ� �̹��� �Ŵ����� �������� �ϴ� �κ� 
/// </summary>
/// <param name="hwnd"></param>
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		// D2D1_BITMAP_OPTIONS_TARGET : ������ Ÿ��(��, �׷����� ���)������ ����ϰڴٴ� �ǹ��Դϴ�. Direct2D�� ID2D1DeviceContext::SetTarget(...)�� �ٷ� ����
		// D2D1_BITMAP_OPTIONS_CANNOT_DRAW : �� ��Ʈ���� Direct2D���� ���� DrawBitmap(...) ���� �Լ��� �׸��� �ʰ�, ���� ���� Ÿ�����θ� ����ϰڴ١��� ��. ��, ȭ�� ��¿����θ� ��� 
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
		// scDesc.Format :  Swap Chain�� ���� �� DXGI_FORMAT_B8G8R8A8_UNORM ���� �����ߴ� ���� �޾ƿ� ��B8G8R8A8(32��Ʈ) ���ֶ�����塱 �������� Direct2D�� ��Ʈ���� �ؼ�
		// D2D1_ALPHA_MODE_PREMULTIPLIED : ����(����) ä���� ������ ����(��������Ƽ�ö��̵� ���ġ�)�� ����� �����͸� ���ٴ� �ǹ�
	);
	// ��� �ȼ� ��������, � �ɼ��� ���� ��Ʈ�ʡ����� �˷��ֱ� ����, Direct2D�� ��Ʈ�� �Ӽ��� �����ϰ�, ����ü D2D1_BITMAP_PROPERTIES1 �� ��´�. 


	g_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, g_d2dBitmapTarget.GetAddressOf()); // ��IDXGISurface�� �ٷ� Direct2D�� ID2D1Bitmap1 ��ü�� ��ȯ(Wrap)�� �޶󡱴� ���
	// ���� GPU �޸��� �ؽ�ó�� �����ϴ� ���� ����, ������ �޸𸮸� ����(shared)���� ����Ѵ�. 

	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	//Direct2D�� ��ġ ���ؽ�Ʈ(ID2D1DeviceContext)�� ���������� �׸� ���(target)�� ��� ������ ��Ʈ������ ��ڴ١�

// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(g_wicImagingFactory),
		(void**)g_wicImagingFactory.GetAddressOf());
	//�̹��� ����(PNG ��)�� �ε��Ͽ� ID2D1Bitmap1 ��ü�� ����� �ʱ�ȭ ����

	hr = CreateBitmapFromFile(L"../Resource/mushroom.png", g_d2dBitmapFromFile.GetAddressOf());
	assert(SUCCEEDED(hr));
	/// <summary>
	/// ��������� �̹��� �Ŵ����� �������� �ϴ� �κ� 
	/// </summary>
	/// <param name="hwnd"></param>
}
