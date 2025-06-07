#include "Winmain.h"


Winmain app; 

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
           break; // �ּ�ȭ�� ����  

       UINT width = LOWORD(lParam); // �� �ʺ�  
       UINT height = HIWORD(lParam); // �� ����  

       // Ư�� Winmain ��ü�� �����ϵ��� ����  
       if (app.g_width != width || app.g_height != height)  
       {  
           app.g_width = width;  
           app.g_height = height;  
           app.g_resized = true;  
       }  
   }  
   break;  
   case WM_EXITSIZEMOVE:  
       if (app.g_resized)  
       {  
           app.Uninitialize();  
           app.Initialize();  
       }  
       break;  
   default:  
       break;  
   }  
   return DefWindowProc(hWnd, msg, wParam, lParam);  
}  

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)  
{  
   app.g_hInstance = hInstance;  
   app.Initialize();  
   ShowWindow(app.g_hwnd, nCmdShow);  

   CoInitialize(nullptr);  

   app.Run();  
   app.Uninitialize();  

   CoUninitialize();  
   return 0;  
}


void Winmain::Run()
{
	// �޽��� ����
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

void Winmain::Initialize()
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

void Winmain::Uninitialize()
{
}

void Winmain::Render() 
{

}
