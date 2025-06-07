#include "RenderManager.h"
// WIC�� ���� PNG ���� �ε��Ͽ� ID2D1Bitmap1**���� ��ȯ
void RenderManager::Initialize()
{
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

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain ���� // ���� ���۸�. �ΰ��� ���۸� ü��ó�� �����ؼ� ����� �� �ְ� �ϴ� ��. 
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;								  // ���� �ʺ�. ���� ���� 
	scDesc.Height = height;								  // ���� ����. ���� ���� 
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			  // �� �ȼ� ���� 
	scDesc.SampleDesc.Count = 1;						  // ��Ƽ ���ø� ��� ���� (1 =  ������)  ��Ƽ ���ø� : �����ڸ��� �ε巯��������.   
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �׸� ���۷� ����Ѵٴ� ����� �ڵ� 
	scDesc.BufferCount = 2;								  // ���� ���۸��� ���� 2���� ���۸� ��� 
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	  // ���۸� ��ü�ϴ� ��� 
	dxgiFactory->CreateSwapChainForHwnd(g_d3dDevice.Get(), hwnd, &scDesc, nullptr, nullptr, g_dxgiSwapChain.GetAddressOf()); // Ư�� ������ â�� ���ؼ� ���� ü���� ���� �ϴ� �� 


	// ����۸� Ÿ������ ����
	ComPtr<IDXGISurface> backBuffer;
	g_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)); // ����� ����۸� ���� ü�ο��� IDXGISurface Ÿ������ �����´�. 

	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	// ��� �ȼ� ��������, � �ɼ��� ���� ��Ʈ�ʡ����� �˷��ֱ� ����, Direct2D�� ��Ʈ�� �Ӽ��� �����ϰ�, ����ü D2D1_BITMAP_PROPERTIES1 �� ��´�. 


	g_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, g_d2dBitmapTarget.GetAddressOf()); // ��IDXGISurface�� �ٷ� Direct2D�� ID2D1Bitmap1 ��ü�� ��ȯ(Wrap)�� �޶󡱴� ���
	// ���� GPU �޸��� �ؽ�ó�� �����ϴ� ���� ����, ������ �޸𸮸� ����(shared)���� ����Ѵ�. 

	g_d2dDeviceContext->SetTarget(g_d2dBitmapTarget.Get());
	//Direct2D�� ��ġ ���ؽ�Ʈ(ID2D1DeviceContext)�� ���������� �׸� ���(target)�� ��� ������ ��Ʈ������ ��ڴ١�
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
	BeginDrawClear(D2D1::ColorF::Black); // ���������� ȭ���� �ʱ�ȭ 

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
	g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ 
	g_d2dDeviceContext->DrawBitmap(Bitmap.Get(), destrect);
}

void RenderManager::EndDraw()
{
	g_d2dDeviceContext->EndDraw();
	g_dxgiSwapChain->Present(1, 0); // ����۸� ���� ���۷� ������. ���. 
}
// ���� ü�ο� �ִ� ����۸� ���� ���۷� ��ü�ϴ� �۾�. 