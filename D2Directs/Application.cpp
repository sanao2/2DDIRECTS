#include "pch.h"
#include "Application.h"
#include <exception>

bool isQuit = true;

void Application::Initialize()
{
	CoInitialize(nullptr);
	g_renderManager.Initialize();
}

void Application::Run()
{
   // 메시지 루프
   try {
       MSG msg = {};
       // 메시지 루프
       while (isQuit)
       {
           while (msg.message != WM_QUIT && PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
               if (msg.message == WM_QUIT) {
                   return;
               }
               TranslateMessage(&msg);
               DispatchMessage(&msg);
           }
           // 렌더 호출

           Render();
       }
   }
   catch (const std::exception& e) {
       // std::exception 계열(예: std::runtime_error)로 throw된 예외 잡기
       MessageBoxA(nullptr, e.what(), "예외 발생", MB_OK | MB_ICONERROR);
   }
   catch (...) {
       // 그 외 모든 예외(정수, 문자열, 사용자 정의 클래스 등) 잡기
       MessageBox(nullptr, L"알 수 없는 예외가 발생했습니다.", L"예외 발생", MB_OK | MB_ICONERROR);
   }
}

void Application::Render()
{	
	g_renderManager.Render(); // 렌더링 매니저의 렌더 메서드 호출

}

void Application::Uninitialize()
{
	g_renderManager.Uninitialize();
   
    CoUninitialize();
}
