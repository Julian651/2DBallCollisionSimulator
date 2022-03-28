#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#include <d2d1.h>
#include <dwrite.h>
template <class T> void SafeRelease(T** ppT)
{
   if (*ppT)
   {
      (*ppT)->Release();
      *ppT = NULL;
   }
}
class Graphics
{
private:
   ID2D1HwndRenderTarget* pRT;
   ID2D1SolidColorBrush* brush;
   ID2D1Factory* pD2DFactory;
   IDWriteFactory* pWriteFact;
   IDWriteTextFormat* pTextFormat;
   ID2D1PathGeometry* pPathGeom;
   HRESULT hr;
   HWND hWnd;
   RECT rc;


   // Create a Direct2D render target          

   void create()
   {

      pD2DFactory = NULL;

      hr = D2D1CreateFactory(
         D2D1_FACTORY_TYPE_SINGLE_THREADED,
         &pD2DFactory);
      pRT = NULL;
      brush = NULL;

      HRESULT hrWrite;

      if (SUCCEEDED(hr))
      {
         hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
            &pRT);
         hrWrite = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(pWriteFact),
            reinterpret_cast<IUnknown**>(&pWriteFact));

         if (SUCCEEDED(hrWrite))
         {
            hrWrite = pWriteFact->CreateTextFormat(
               L"Verdana", NULL,
               DWRITE_FONT_WEIGHT_NORMAL,
               DWRITE_FONT_STYLE_NORMAL,
               DWRITE_FONT_STRETCH_NORMAL,
               50,
               L"",
               &pTextFormat
            );
         }

         if (SUCCEEDED(hrWrite))
         {
            pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
            pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
         }
      }



      if (SUCCEEDED(hr))
      {
         pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &brush
         );
      }
   }
public:
   Graphics(HWND hWnd) : hWnd{ hWnd }
   {
      getClippingArea();
      create();
   }
   ~Graphics()
   {
      SafeRelease(&pRT);
      SafeRelease(&brush);
      SafeRelease(&pD2DFactory);
      SafeRelease(&pWriteFact);
      SafeRelease(&pTextFormat);
      SafeRelease(&pPathGeom);
   }
   RECT getClippingArea()
   {
      // Obtain the size of the drawing area.
      GetClientRect(hWnd, &rc);
      return rc;
   }

   void beginDraw()
   {
      pRT->BeginDraw();
   }
   void endDraw()
   {
      pRT->EndDraw();
   }
   void clear()
   {
      pRT->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
   }
   void setBrush(D2D1::ColorF color)
   {
      if (brush == NULL)
      {
         pRT->CreateSolidColorBrush(
            D2D1::ColorF(color),
            &brush
         );
      }
      else
      {
         brush->SetColor(color);
      }
   }

   void DrawRect(float x, float y, float width, float height)
   {
      if (pRT != NULL && brush != NULL)
      {
         getClippingArea();

         pRT->DrawRectangle(
            D2D1::RectF(
               x,
               y,
               width,
               height),
            brush);
      }
   }
   void DrawCircle(float cx, float cy, float r)
   {
      if (pRT != NULL && brush != NULL)
      {
         getClippingArea();

         pRT->DrawEllipse(
            D2D1::Ellipse(D2D1::Point2(cx, cy), r, r),
            brush
         );

      }
   }
   void FillCircle(float cx, float cy, float r)
   {
      if (pRT != NULL && brush != NULL)
      {
         getClippingArea();

         pRT->FillEllipse(
            D2D1::Ellipse(D2D1::Point2(cx, cy), r, r),
            brush
         );

      }
   }
   void DrawString(const wchar_t* text, float x, float y, float fontSize)
   {
      if (pWriteFact != NULL)
      {
         IDWriteTextLayout* pTextLayout;

         int stringLength = wcslen(text);
         HRESULT hres = pWriteFact->CreateTextLayout(
            text, stringLength,
            pTextFormat,
            (float)stringLength * fontSize / 1.75f,
            0.0f,
            &pTextLayout);

         if (SUCCEEDED(hres))
         {

            DWRITE_TEXT_RANGE allText = { (UINT32)0, (UINT32)stringLength };
            pTextLayout->SetFontSize(fontSize, allText);

            pRT->DrawTextLayout(
               D2D1::Point2F(x, y),
               pTextLayout,
               brush,
               D2D1_DRAW_TEXT_OPTIONS_NONE
            );

            SafeRelease(&pTextLayout);
         }
      }
   }
};