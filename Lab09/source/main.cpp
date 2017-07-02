#include <Windows.h>
#include "res.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#define ID_TIMER_REDRAW 10

BYTE* ImageBufferY;
BYTE* ImageBufferU;
BYTE* ImageBufferV;
BYTE* ImageBufferRGB;
HANDLE hfile;
HBITMAP hBmp;
HDC hdcMemDC;
int losowa, licznik = 0;


__int64 iStart;

__int64 iEnd;

INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {

    case BN_CLICKED:  //zdarzenie klikniecia na UI
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1: //klikniecie na naszym przycisku
          HWND hwndEditBox = GetDlgItem(hwndDig, IDC_EDIT1);  //Pobranie uchwytu kontrolki 
          HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1);
          HWND hwndStatic1 = GetDlgItem(hwndDig, IDC_STATIC1);
          HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
          CHAR szText[500];   // tablica dla EditBox
          CHAR lText[500];  //tablica dla licznika
          int iTextLength = GetWindowTextLength(hwndEditBox); //Pobranie dlugosci tekstu
          GetWindowText(hwndEditBox, szText, iTextLength + 1); //Pobranie tekstu
          int liczba = atoi(szText);
          if ((liczba > 0) && (liczba <= 40)) {
            if (liczba > losowa) {
              licznik++;
              wsprintf(lText, "Próba: %d", licznik);
              SetWindowText(hwndStatic1, lText); //Przypisanie go do przycisku
              SetWindowText(hwndStatic2,"Twoja liczba jest za du¿a"); //Przypisanie go do przycisku
            }
            else if (liczba < losowa) {
              licznik++;
              wsprintf(lText, "Próba: %d", licznik);
              SetWindowText(hwndStatic1, lText); //Przypisanie go do przycisku
              SetWindowText(hwndStatic2, "Twoja liczba jest za ma³a"); //Przypisanie go do przycisku
            }
            else {
              licznik++;
              wsprintf(lText, "Próba: %d", licznik);
              SetWindowText(hwndStatic1, lText); //Przypisanie go do przycisku
              SetWindowText(hwndStatic2, "Zwyciêstwo"); //Przypisanie go do przycisku
              wsprintf(szText, "Wygra³eœ", LOWORD(lParam), HIWORD(lParam));
              MessageBox(hwndDig, szText, TEXT("Werdykt"), MB_OK);
            }
          }
          else {
            wsprintf(szText, "Coœ Ci nie posz³o");
            MessageBox(hwndDig, szText, TEXT("ERROR 404 Not FOUND"), MB_OK);
          }

        return TRUE;

        }
      }
      return FALSE;



  case WM_TIMER:
  {
    switch (wParam)
    {
    case ID_TIMER_REDRAW:
      if (!hfile)break;

      DWORD ret;

      ReadFile(hfile, ImageBufferY,352*288,&ret,NULL);
      ReadFile(hfile, ImageBufferU, 352 * 288/4, &ret, NULL);
      ReadFile(hfile, ImageBufferV, 352 * 288/4, &ret, NULL);

      //conversion from YUV to RGB
      //iStart = GetTickCount(); //mierzymy czas systemowy

      QueryPerformanceCounter((_LARGE_INTEGER*)&iStart);  //zegar procesora wynik w us
      for (int y = 0; y < 288; y++) 
      {
        for (int x = 0; x < 352; x++)
        {
          int t = y / 2 * 352 / 2 + x / 2;
          int yy = ImageBufferY[y * 352 + x];
          int uu = ImageBufferU[t]-128;
          int vv = ImageBufferV[t]-128;

          int e = 256 * yy;
          int rr = (256 * 1.40200);
          int gg1 = (256 * -0.34414);
          int gg2 = (256 * -0.71414);
          int bb = (256 * 1.77200);

          int r = ((rr*vv + e)) >> 8;
          int g= (gg1*uu+gg2*vv + e) >>8;
          int b= (bb*uu + e) >> 8;

          int w = 3 * ((287 - y) * 352 + x);   // (287-y) wpisywanie od koñca aby obraz nie by³ odwrócony 
          ImageBufferRGB[w + 0] = b;  // 0 1 2 to jest offset
          ImageBufferRGB[w + 1] = g;
          ImageBufferRGB[w + 2] = r;
        }
      }
      //iEnd = GetTickCount();
      QueryPerformanceCounter((_LARGE_INTEGER*)&iEnd);
      RECT rc;
      GetClientRect(hwndDig, &rc);
      InvalidateRect(hwndDig, NULL, false);
      RedrawWindow(hwndDig, &rc, NULL, RDW_ERASE);
      break;
    }
    return FALSE;
  }

  case WM_PAINT:
  {
    static int x = 0;
    HDC hdc = GetDC(hwndDig);

    HDC hdcMemDC;
    hdcMemDC = CreateCompatibleDC(hdc);

    HBITMAP hBmp;
    hBmp = CreateCompatibleBitmap(hdc, 352, 288);
    SelectObject(hdcMemDC, hBmp);
    
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = 352;
    bi.biHeight = 288;   //sprytna metoda - i bedzie obrócona 
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 352*288;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    //przypisanie bufora kolorów
    SetDIBits(hdcMemDC, hBmp, 0, 288, ImageBufferRGB, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    //Wczyt obrazka
    BitBlt(hdc, 0, 0, 352, 288, hdcMemDC, 0, 0, SRCCOPY);
    char text[255];
    sprintf_s(text, "%d", iEnd - iStart);
    //TextOut(hdc, x, 10, "0_0", 3);
    TextOut(hdc, x, 10, text,strlen(text));
    //x = x + 1;
    ReleaseDC(hwndDig, hdc);
  }
  return FALSE;

  case WM_INITDIALOG:
    {
    hfile = CreateFile("..//res//claire.cif", 
                   GENERIC_READ, 
                   0,
                   (LPSECURITY_ATTRIBUTES)0,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   (HANDLE)NULL);// GR -tryb dostepu do pliku, FILLE_ATTR czy systemowy/specjalny

    ImageBufferY = (BYTE*)malloc(352 * 288);
    ImageBufferU = (BYTE*)malloc(352 * 288/4);
    ImageBufferV = (BYTE*)malloc(352 * 288/4);

    ImageBufferRGB = (BYTE*)malloc(352 * 288 * 4);
    return FALSE;
    }

    case WM_CLOSE:
      free(ImageBufferRGB);
      free(ImageBufferY);
      free(ImageBufferU);
      free(ImageBufferV);
      CloseHandle(hfile); //zamyka pliki
      DestroyWindow(hwndDig);  //zniszczenie okienka
      PostQuitMessage(0); //umieszczenie w pêtli komunikatów polecenia zakonczenia aplikacji
      return TRUE;
    }
    return FALSE;
  }


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  SetTimer(hwndMainWindow, ID_TIMER_REDRAW,1, NULL);
  srand((time(NULL)));
  losowa = rand() % 40 + 1;

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}


//fragment kodu z tymi macierzami aby by³o szybciej


//3*()  -robimy trzy razy
//256*yy
//dzielenie i mno¿enie na operacje bitowee