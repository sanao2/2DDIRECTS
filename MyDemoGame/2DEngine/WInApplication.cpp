#include "pch.h"
#include "WInApplication.h"


void WInApplication::Run()
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

void WInApplication::Initialize()
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

}

void WInApplication::Uninitialize()
{
}

void WInApplication::Render()
{

}