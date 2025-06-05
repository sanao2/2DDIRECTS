#pragma once  
#include "../2DEngine/pch.h"  
#include "../2DEngine/RenderManager.h"  
#include "resource.h"  

class Application  
{  
private:  
   ComPtr<ID2D1DeviceContext7> g_d2dDeviceContext;
   ComPtr<ID2D1Bitmap1> g_d2dBitmapFromFile;  
   ComPtr<IWICImagingFactory> g_wicImagingFactory;  

   // RenderManager를 전방 선언 대신 포함하여 불완전한 형식 문제 해결  
   RenderManager* pRManager;  

public:  
   Application() : pRManager(nullptr) {}  
   ~Application() { if (pRManager) delete pRManager; }  

   void Initialize();  
   void Uninitialize();  
   void Render();  

   void CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);  
};