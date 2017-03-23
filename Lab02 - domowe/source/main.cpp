#include <Windows.h>
#include "res.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>


int losowa, licznik = 0;

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