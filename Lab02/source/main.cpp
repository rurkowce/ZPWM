#include <Windows.h>
#include <stdio.h>
#include "res.h"

INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    
      case EN_CHANGE:
        switch (LOWORD(wParam)) {
        case IDC_EDIT1: //klikniecie na naszym przycisku
          //MessageBox(hwndDig, TEXT("Test"), TEXT("Klikniecie"), MB_OK);
          HWND hwndEditBox = GetDlgItem(hwndDig, IDC_EDIT1);  //Pobranie uchwytu kontrolki IDC_EDIT1
          int iTextLength = GetWindowTextLength(hwndEditBox); //Pobranie dlugosci tekstu
          CHAR szText[500];
          GetWindowText(hwndEditBox, szText, iTextLength + 1); //Pobranie tekstu
          HWND hwndButton = GetDlgItem(hwndDig, IDC_BUTTON1);  //Pobranie uchwytu przycisku
          SetWindowText(hwndButton, szText); //Przypisanie go do przycisku
          return TRUE;
        }
        return FALSE;

      case BN_CLICKED:  //zdarzenie klikniecia na UI
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1: //klikniecie na naszym przycisku
        MessageBox(hwndDig, TEXT("Test"), TEXT("Klikniecie"), MB_OK);

        HWND hwndEditBox = GetDlgItem(hwndDig, IDC_EDIT1);  //Pobranie uchwytu kontrolki IDC_EDIT1
          int iTextLength = GetWindowTextLength(hwndEditBox); //Pobranie dlugosci tekstu
        CHAR szText[500];
        GetWindowText(hwndEditBox, szText, iTextLength + 1); //Pobranie tekstu
        SetWindowText((HWND)lParam, szText); //Przypisanie go do przycisku
        return TRUE;
      }
    }
    return FALSE;

  case WM_LBUTTONDOWN:
    CHAR szText[200];
    wsprintf(szText, "Kliknales myszka x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDig, szText, TEXT("Klikniecie"), MB_OK);
    return TRUE;

  case WM_CLOSE:
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

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}