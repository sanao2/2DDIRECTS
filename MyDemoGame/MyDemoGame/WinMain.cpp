#include "Winmain.h"


Application apps; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)  
{   
	apps.Initialize(nCmdShow);

   CoInitialize(nullptr);  

   apps.Run();  
   apps.Uninitialize();

   CoUninitialize();  
   return 0;  
}

