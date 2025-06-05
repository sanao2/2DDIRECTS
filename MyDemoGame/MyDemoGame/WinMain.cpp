#include "Winmain.h"

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
			//Winmain::Uninitialize();
			//Winmain::Initialize(hWnd);
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	g_hInstance = hInstance;
	Winmain app;
	app.Initialize();
	ShowWindow(g_hwnd, nCmdShow);

	CoInitialize(nullptr);

	app.Run();
	app.Uninitialize();

	CoUninitialize();
	return 0;
}
