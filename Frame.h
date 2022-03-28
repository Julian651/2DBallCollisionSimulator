#pragma once
#include "Graphics.h"

class Frame
{
private:
   static const wchar_t* SZ_CLASS_NAME;
   static const wchar_t* SZ_TITLE;
   static int nWidth;
   static int nHeight;
   static HWND hWnd;
   static HINSTANCE hIns;
   static Graphics* graphics;
public:
   static void createWindow(HINSTANCE hInstance, const wchar_t* szClassName, const wchar_t* szTitle, int width, int height);
   static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
   static void setVisible(bool visible);
   static void paint(Graphics* g);
   static void repaint();
   static Graphics* getGraphics() { return graphics; }
   static int getWidth() { return nWidth; }
   static int getHeight() { return nHeight; }


};

