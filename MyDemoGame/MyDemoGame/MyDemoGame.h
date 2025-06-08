#include "../2DEngine/pch.h"  
#pragma once  
#include "../2DEngine/RenderManager.h"  
#include "resource.h"  
extern HWND g_hwnd;
extern HINSTANCE g_hInstance;
extern UINT g_width;
extern UINT g_height;

class Application 
{  
private:  
   ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
   ComPtr<ID2D1Bitmap1> g_d2dBitmapFromFile;  
   ComPtr<IWICImagingFactory> g_wicImagingFactory;  

   // RenderManager를 전방 선언 대신 포함하여 불완전한 형식 문제 해결  
   RenderManager* pRManager = nullptr;  

public:  
   Application() : pRManager(new RenderManager(g_hwnd, g_width, g_height, g_d2dDeviceContext)) {}
   ~Application() { if (pRManager) delete pRManager; }  

   void Initialize();
   void Uninitialize() ;
   void Render() ;
   void Run();
   
   void LoadeImageFromFile(const wchar_t* path);
   HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
};