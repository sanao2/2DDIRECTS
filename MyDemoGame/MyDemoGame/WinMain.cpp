#include "Winmain.h"


Application apps; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)  
{   

   apps.Initialize();
   ShowWindow(g_hwnd, nCmdShow);

   apps.Run();  
   apps.Uninitialize();

   CoUninitialize();  
   return 0;  
}

