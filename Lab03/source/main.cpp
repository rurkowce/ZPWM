#include <Windows.h>
#include <stdio.h>
#include "res.h"

bool isGameOn = false;
bool isFirstPlayerTurn = true;
bool remis = false;
int licznik = 0;
int tab[9] = { 2,3,4,5,6,7,8,9,10 };

bool isResult(HWND hwndDig, HWND param, int znak)
{

  HWND hwndButton2 = GetDlgItem(hwndDig, IDC_BUTTON2);
  HWND hwndButton3 = GetDlgItem(hwndDig, IDC_BUTTON3);
  HWND hwndButton4 = GetDlgItem(hwndDig, IDC_BUTTON4);
  HWND hwndButton5 = GetDlgItem(hwndDig, IDC_BUTTON5);
  HWND hwndButton6 = GetDlgItem(hwndDig, IDC_BUTTON6);
  HWND hwndButton7 = GetDlgItem(hwndDig, IDC_BUTTON7);
  HWND hwndButton8 = GetDlgItem(hwndDig, IDC_BUTTON8);
  HWND hwndButton9 = GetDlgItem(hwndDig, IDC_BUTTON9);
  HWND hwndButton10 = GetDlgItem(hwndDig, IDC_BUTTON10);

  if (znak == 0) {
    if (hwndButton2 == param) tab[0] = 0;
    if (hwndButton3 == param) tab[1] = 0;
    if (hwndButton4 == param) tab[2] = 0;
    if (hwndButton5 == param) tab[3] = 0;
    if (hwndButton6 == param) tab[4] = 0;
    if (hwndButton7 == param) tab[5] = 0;
    if (hwndButton8 == param) tab[6] = 0;
    if (hwndButton9 == param) tab[7] = 0;
    if (hwndButton10 == param) tab[8] = 0;
  }
    else {
      if (hwndButton2 == param) tab[0] = 1;
      if (hwndButton3 == param) tab[1] = 1;
      if (hwndButton4 == param) tab[2] = 1;
      if (hwndButton5 == param) tab[3] = 1;
      if (hwndButton6 == param) tab[4] = 1;
      if (hwndButton7 == param) tab[5] = 1;
      if (hwndButton8 == param) tab[6] = 1;
      if (hwndButton9 == param) tab[7] = 1;
      if (hwndButton10 == param) tab[8] = 1;
    }

  if ((tab[0] == tab[1]) && (tab[0] == tab[2]) ||
    (tab[3] == tab[4]) && (tab[3] == tab[5]) ||
    (tab[6] == tab[7]) && (tab[6] == tab[8]) ||
    (tab[0] == tab[4]) && (tab[0] == tab[8]) ||
    (tab[2] == tab[4]) && (tab[2] == tab[6]) ||
    (tab[0] == tab[3]) && (tab[0] == tab[6]) ||
    (tab[1] == tab[4]) && (tab[1] == tab[7]) ||
    (tab[2] == tab[5]) && (tab[2] == tab[8]))
  {
    return TRUE;
  }
  else {
    licznik++;
    if (licznik == 9) remis = true;
    return FALSE;
  }
}




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
        case IDC_BUTTON2:
        case IDC_BUTTON3:
        case IDC_BUTTON4:
        case IDC_BUTTON5:
        case IDC_BUTTON6:
        case IDC_BUTTON7:
        case IDC_BUTTON8:
        case IDC_BUTTON9:
        case IDC_BUTTON10:
          if(isGameOn==true)
        {
          //isFirstPlayerTurn
          HWND hwndButton = (HWND)lParam;
          if (GetWindowTextLength(hwndButton) == 0)
          {
            HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
            CHAR szText[500];
            if (isFirstPlayerTurn == true) {
              wsprintf(szText, "X");
              HWND hwndRadio1 = GetDlgItem(hwndDig, IDC_RADIO1);
              CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);

              if (isResult (hwndDig,hwndButton, 0)) {
                CHAR szText[500];
                wsprintf(szText, "Wygra³ X");
                SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                isGameOn = false;
              }
              else if (remis) {
                CHAR szText[500];
                wsprintf(szText, "Remis");
                SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                isGameOn = false;
              }

            }
            else {
              wsprintf(szText, "O");
              HWND hwndRadio1 = GetDlgItem(hwndDig, IDC_RADIO2);
              CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
              if (isResult(hwndDig, hwndButton, 1)) {
                CHAR szText[500];
                wsprintf(szText, "Wygra³ O");
                SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku

              }
              else if (remis) {
                CHAR szText[500];
                wsprintf(szText, "Remis");
                SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                isGameOn = false;
              }
            }
            SetWindowText(hwndButton, szText);
            isFirstPlayerTurn = !isFirstPlayerTurn;
          }
          return TRUE;
        }
          return TRUE;

      case IDC_BUTTON1: //klikniecie na naszym przycisku NOWA GRA

        if (isGameOn == false)
        {
          licznik = 0;
          remis = false;
          for (int i = 2; i < 11; i++) tab[i - 2] = i;
          isFirstPlayerTurn = true;
          CHAR szText[500];
          HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
          wsprintf(szText, "Stan gry");
          SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
          isGameOn = true;

          for (int i = 0; i < 9; i++)
          {
            HWND hwndButton = GetDlgItem(hwndDig, IDC_BUTTON2 + i);  //Pobranie uchwytu kontrolki Buttonów
            CHAR szText[500];
            wsprintf(szText, "");
            SetWindowText(hwndButton, szText); //Przypisanie go do przycisku
          }
          HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1);  //Pobranie uchwytu kontrolki IDC_BUTTON1
          //CHAR szText[500];
          wsprintf(szText, "Stop");
          SetWindowText(hwndButton1, szText); //Przypisanie go do przycisku
        }
        else
        {
          HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1);  //Pobranie uchwytu kontrolki IDC_BUTTON1
          CHAR szText[500];
          wsprintf(szText, "Start");
          SetWindowText(hwndButton1, szText); //Przypisanie go do przycisku
          isGameOn = false;
        }
        return TRUE;
      }
        return TRUE;
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

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}