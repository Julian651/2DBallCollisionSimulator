#include "Frame.h"
#include "Ball.h"
#include <iostream>

const wchar_t* SZ_CLASS_NAME = L"Class Name";
const wchar_t* SZ_TITLE = L"Title";

int CALLBACK WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR lpCmdLine,
   _In_ int nShowCmd
)
{
   // uncomment below for debugging console
   /*if (AllocConsole() == 0)
   {
      // Handle error here. Use ::GetLastError() to get the error.
   }

   // Redirect CRT standard input, output and error handles to the console window.
   FILE* pNewStdout = nullptr;
   FILE* pNewStderr = nullptr;
   FILE* pNewStdin = nullptr;

   ::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
   ::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);
   ::freopen_s(&pNewStdin, "CONIN$", "r", stdin);

   // Clear the error state for all of the C++ standard streams. Attempting to accessing the streams before they refer
   // to a valid target causes the stream to enter an error state. Clearing the error state will fix this problem,
   // which seems to occur in newer version of Visual Studio even when the console has not been read from or written
   // to yet.
   std::cout.clear();
   std::cerr.clear();
   std::cin.clear();

   std::wcout.clear();
   std::wcerr.clear();
   std::wcin.clear();*/
   Frame::createWindow(hInstance, L"TESTNAME", L"Physics", 800, 800);
   Frame::setVisible(true);
   //Graphics* g = fr->getGraphics();
   //g->DrawRect(0, 0, 100, 100);



   MSG msg;

   

   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return msg.wParam;
   
}


