#include "MyDemoGame.h"

// 전역 변수
// UnInitlialize() 함수 호출 이후 해제해야함 
// Relese() 함수 호출 

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
			//Uninitialize();
			//Application::Initialize(hWnd);
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void Application::Initialize(HWND hwnd, UINT width, UINT height)
{

}

void Application::Uninitialize()
{
	g_d2dBitmapFromFile = nullptr;

	g_d3dDevice = nullptr;
	g_dxgiSwapChain = nullptr;
	g_d2dDeviceContext = nullptr;
	g_d2dBitmapTarget = nullptr;
}

void Application::Render()
{
	g_d2dDeviceContext->BeginDraw();
	g_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	D2D1_SIZE_F size;

	g_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화

	//1. 0,0 위치에 비트맵 전체영역 그린다. (변환은 초기화)
	g_d2dDeviceContext->DrawBitmap(g_d2dBitmapFromFile.Get());

	//2. DestPos(화면 위치) 설정과 SrcPos(비트맵 위치)로 그리기
	D2D1_VECTOR_2F DestPos{ 0,0 }, SrcPos{ 0,0 }; // 화면 위치, 비트맵 위치
	size = { 0,0 };	//	그릴 크기
	D2D1_RECT_F DestRect{ 0,0,0,0 }, SrcRect{ 0,0,0,0 }; // 화면 영역, 비트맵 영역
	D2D1_MATRIX_3X2_F transform;	// 변환 행렬

	size = g_d2dBitmapFromFile->GetSize();
	DestPos = { 100,0 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };
	g_d2dDeviceContext->DrawBitmap(g_d2dBitmapFromFile.Get(), DestRect);


	//3. DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	size = { 200,200 };
	DestPos = { 100,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	SrcPos = { 0,0 };
	SrcRect = { SrcPos.x,SrcPos.y, SrcPos.x + size.width - 1 ,SrcPos.y + size.height - 1 };
	g_d2dDeviceContext->DrawBitmap(g_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);


	//4. 변환을 사용한 반전으로 DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	DestPos = { 700,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(-1.0f, 1.0f,  // x축 반전
		D2D1::Point2F(DestPos.x, DestPos.y));        // 기준점
	g_d2dDeviceContext->SetTransform(transform);
	g_d2dDeviceContext->DrawBitmap(g_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);

	//5. 복합변환을 사용한 반전으로 DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	DestPos = { 0,0 };   // 그릴 위치는 0,0으로 하고 이동변환을 사용한다.
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f, D2D1::Point2F(0.0f, 0.0f)) *
		D2D1::Matrix3x2F::Rotation(90.0f, D2D1::Point2F(0.0f, 0.0f)) * // 90도 회전
		D2D1::Matrix3x2F::Translation(900.0f, 900.0f);  // 이동

	// 기준점
	g_d2dDeviceContext->SetTransform(transform);
	g_d2dDeviceContext->DrawBitmap(g_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);

	g_d2dDeviceContext->EndDraw();
	g_dxgiSwapChain->Present(1, 0);
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

int WINAPI Application::WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MyD2DWindowClass";
	RegisterClass(&wc);

	SIZE clientSize = { (LONG)g_width,(LONG)g_height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	g_hwnd = CreateWindowEx(0, L"MyD2DWindowClass", L"D2D1 Clear Example",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
		nullptr, nullptr, hInstance, nullptr);
	ShowWindow(g_hwnd, nCmdShow);

	CoInitialize(nullptr);
	Initialize(g_hwnd, g_width, g_height);

	Run(); 

	Uninitialize();
	CoUninitialize();
	return 0;
}
