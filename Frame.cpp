#include "Frame.h"

#include "Simulation.h"
#include <thread>

const wchar_t* Frame::SZ_CLASS_NAME = L"TEST";
const wchar_t* Frame::SZ_TITLE = L"TEST2";
int Frame::nWidth = 100;
int Frame::nHeight = 100;
HWND Frame::hWnd = NULL;
HINSTANCE Frame::hIns = NULL;
Graphics* Frame::graphics = NULL;

Simulation sim;
std::thread thread_sim(&Simulation::run, &sim);

void Frame::createWindow(HINSTANCE hInstance, const wchar_t* szClassName, const wchar_t* szTitle, int width, int height)
{
   hIns = hInstance;

   SZ_CLASS_NAME = szClassName;
   SZ_TITLE = szTitle;
   
   nWidth = width;
   nHeight = height;

   WNDCLASSEX wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wcex.lpszMenuName = NULL;
   wcex.lpszClassName = szClassName;
   wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

   if (!RegisterClassEx(&wcex))
   {
      MessageBox(NULL,
         L"Call to RegisterClassEx failed!",
         L"Windows Desktop Guided Tour",
         NULL);
      return;
   }

   HWND hwnd = CreateWindow(
      szClassName,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      0, 0,
      width, height,
      NULL,
      NULL,
      hInstance,
      NULL
   );
   if (!hwnd)
   {
      MessageBox(NULL,
         L"Call to CreateWindow failed!",
         L"Windows Desktop Guided Tour",
         NULL);
      return;
   }
   hWnd = hwnd;
   graphics = new Graphics(hwnd);
}

LRESULT CALLBACK Frame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
   case WM_CREATE:
      // Initialize the window. 
      sim.start();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      sim.pause();
      return 0;

   case WM_PAINT:
      // Paint the window's client area. 
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      paint(graphics);
      EndPaint(hWnd, &ps);
      return 0;

   case WM_SIZE:
      // Set the size and position of the window. 
      return 0;

   case WM_DESTROY:
      // Clean up window-specific data objects. 
      sim.stop();
      thread_sim.join();

      delete graphics;
      PostQuitMessage(0);
      return 0;

   case WM_KEYDOWN:
      if (wParam == VK_SPACE)
      {
         if (sim.isAlive())
         {
            if (sim.isStarted()) sim.pause();
            else sim.start();
         }
      }
      if (wParam == VK_ESCAPE)
      {
         DestroyWindow(hWnd);
      }
      return 0;
      // 
      // Process other messages. 
      // 

   case WM_LBUTTONDOWN:
      sim.addBall(LOWORD(lParam), HIWORD(lParam));

      return 0;

   default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
   }
   return 0;
}

void Frame::setVisible(bool visible)
{
   if (visible)
   {
      ShowWindow(hWnd, SW_SHOW);
   }
   else
   {
      ShowWindow(hWnd, SW_HIDE);
   }
}
void Frame::paint(Graphics* g)
{
   //graphics->beginDraw();
   //graphics->clear();

   //graphics->endDraw();
   sim.paint();
}

void Frame::repaint()
{
   SendMessage(hWnd, WM_PAINT, NULL, NULL);
}