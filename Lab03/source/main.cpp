#include <Windows.h>
#include <stdio.h>
#include "res.h"

bool isGameOn = false;
bool isFirstPlayerTurn = true;
bool remis = false;
int licznik = 0;
int tab[9] = { 2,3,4,5,6,7,8,9,10 };
int emptyField = 0;
int iPlace = 0;
bool first = true;
bool isFree = false;

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
bool isResult2()
{
  emptyField++;
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
void isComputer(HWND hwndDig, int znak)
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
  CHAR szText[500];

  for (int i = 0; i < 3; i++)
  {
    CHAR szText[500];
    //wiersze 
    if (tab[3 * i] == 0 && tab[3 * i + 1] == 0 && tab[3 * i + 2] != 0 && tab[3 * i + 2] != 1 ||
      tab[3 * i] == 1 && tab[3 * i + 1] == 1 && tab[3 * i + 2] != 0 && tab[3 * i + 2] != 1)
    {
      if (i == 0)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton4, szText);
        tab[2] = 1;
      }
      if (i == 1)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton7, szText);
        tab[5] = 1;
      }
      if (i == 2)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton10, szText);
        tab[8] = 1;
      }
      return;
    }
    if (tab[3 * i] != 0 && tab[3 * i] != 0 && tab[3 * i + 1] == 0 && tab[3 * i + 2] == 0 ||
      tab[3 * i] != 1 && tab[3 * i] != 1 && tab[3 * i + 1] == 1 && tab[3 * i + 2] == 0 && tab[3 * i + 2] == 1)
    {
      if (i == 0)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton2, szText);
        tab[0] = 1;
      }
      if (i == 1)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton5, szText);
        tab[3] = 1;
      }
      if (i == 2)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton8, szText);
        tab[6] = 1;
      }
      return;
    }
    if (tab[3 * i] == 0 && tab[3 * i + 1] != 0 && tab[3 * i + 1] != 1 && tab[3 * i + 2] == 0 ||
      tab[3 * i] == 1 && tab[3 * i + 1] != 0 && tab[3 * i + 1] != 1 && tab[3 * i + 2] == 1)
    {
      if (i == 0)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton3, szText);
        tab[1] = 1;
      }
      if (i == 1)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton6, szText);
        tab[4] = 1;
      }
      if (i == 2)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton9, szText);
        tab[7] = 1;
      }
      return;
    }

    //kolumny
    if (tab[i] == 0 && tab[i + 3] == 0 && tab[i + 6] != 0 && tab[i + 6] != 1 ||
      tab[i] == 1 && tab[i + 3] == 1 && tab[i + 6] != 0 && tab[i + 6] != 1)
    {
      if (i == 0)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton8, szText);
        tab[6] = 1;
      }
      if (i == 1)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton9, szText);
        tab[7] = 1;
      }
      if (i == 2)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton10, szText);
        tab[8] = 1;
      }
      return;
    }
    if (tab[i] == 0 && tab[i + 3] != 0 && tab[i + 3] != 1 && tab[i + 6] == 0 ||
      tab[i] == 1 && tab[i + 3] != 0 && tab[i + 3] != 1 && tab[i + 6] == 1)
    {
      if (i == 0)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton5, szText);
        tab[3] = 1;
      }
      if (i == 1)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton6, szText);
        tab[4] = 1;
      }
      if (i == 2)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton7, szText);
        tab[5] = 1;
      }
      return;
    }
    if (tab[i] != 0 && tab[i] != 1 && tab[i + 3] == 0 && tab[i + 6] == 0 ||
      tab[i] != 0 && tab[i] != 1 && tab[i + 3] == 1 && tab[i + 6] == 1)
    {
      if (i == 0)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton2, szText);
        tab[0] = 1;
      }
      if (i == 1)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton3, szText);
        tab[1] = 1;
      }
      if (i == 2)
      {
        wsprintf(szText, "O");
        SetWindowText(hwndButton4, szText);
        tab[2] = 1;
      }
      return;
    }
  }
  //skos
  if (tab[0] != 0 && tab[0] != 1 && tab[4] == 0 && tab[8] == 0 ||
    tab[0] != 0 && tab[0] != 1 && tab[4] == 1 && tab[8] == 1)
  {
    wsprintf(szText, "O");
    SetWindowText(hwndButton2, szText);
    tab[0] = 1;
    return;
  }
  if (tab[0] == 0 && tab[4] != 0 && tab[4] != 1 && tab[8] == 0 ||
    tab[0] == 0 && tab[4] != 0 && tab[4] != 1 && tab[8] == 1)
  {
    wsprintf(szText, "O");
    SetWindowText(hwndButton6, szText);
    tab[4] = 1;
    return;
  }
  if (tab[0] == 0 && tab[4] == 0 && tab[8] != 1 && tab[8] != 0 ||
    tab[0] == 1 && tab[4] == 1 && tab[8] != 1 && tab[8] != 0)
  {
    wsprintf(szText, "O");
    SetWindowText(hwndButton10, szText);
    tab[8] = 1;
    return;
  }
  if (tab[2] == 0 && tab[4] == 0 && tab[6] != 1 && tab[6] != 0 ||
    tab[2] == 1 && tab[4] == 1 && tab[6] != 1 && tab[6] != 0)
  {
    wsprintf(szText, "O");
    SetWindowText(hwndButton8, szText);
    tab[6] = 1;
    return;
  }
  if (tab[2] == 0 && tab[6] == 0 && tab[4] != 1 && tab[4] != 0 ||
    tab[2] == 1 && tab[6] == 1 && tab[4] != 1 && tab[4] != 0)
  {
    wsprintf(szText, "O");
    SetWindowText(hwndButton6, szText);
    tab[4] = 1;
    return;
  }
  if (tab[2] == 0 && tab[4] == 0 && tab[2] != 1 && tab[2] != 0 ||
    tab[2] == 1 && tab[4] == 1 && tab[2] != 1 && tab[2] != 0)
  {
    wsprintf(szText, "O");
    SetWindowText(hwndButton4, szText);
    tab[2] = 1;
    return;
  }

  bool empty = false;
  if (tab[4] != 0 && first)
  {
    tab[4] = 1;
    wsprintf(szText, "O");
    SetWindowText(hwndButton6, szText);
    first = false;
  }
  else
  {
    while (empty==false && emptyField!= 9)
    {
      iPlace = rand() % 9;
      while (tab[iPlace]==0 &&emptyField<9)
      {
        iPlace = rand() % 9;
      }
      if (iPlace == 0)
      {
        if (GetWindowTextLength(hwndButton2) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton2, szText);
          empty = true;
        }
      }

      if (iPlace == 1)
      {
        if (GetWindowTextLength(hwndButton3) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton3, szText);
          empty = true;
        }
      }
      if (iPlace == 2)
      {
        if (GetWindowTextLength(hwndButton4) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton4, szText);
          empty = true;
        }
      }
      if (iPlace == 3)
      {
        if (GetWindowTextLength(hwndButton5) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton5, szText);
          empty = true;
        }
      }
      if (iPlace == 4)
      {
        if (GetWindowTextLength(hwndButton6) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton6, szText);
          empty = true;
        }
      }
      if (iPlace == 5)
      {
        if (GetWindowTextLength(hwndButton7) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton7, szText);
          empty = true;
        }
      }
      if (iPlace == 6)
      {
        if (GetWindowTextLength(hwndButton8) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton8, szText);
          empty = true;
        }
      }
      if (iPlace == 7)
      {
        if (GetWindowTextLength(hwndButton9) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton9, szText);
          empty = true;
        }
      }
      if (iPlace == 8)
      {
        if (GetWindowTextLength(hwndButton10) == 0)
        {
          tab[iPlace] = 1;
          wsprintf(szText, "O");
          SetWindowText(hwndButton10, szText);
          empty = true;
        }
      }
    }
  }
}


INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (IsDlgButtonChecked(hwndDig, IDC_RADIO3)) {
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
          if (isGameOn == true)
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

                if (isResult(hwndDig, hwndButton, 0)) {
                  CHAR szText[500];
                  wsprintf(szText, "Wygrał X");
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
                  wsprintf(szText, "Wygrał O");
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
            iPlace = 0;
            isFree = false;
            first = true;
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
  if (IsDlgButtonChecked(hwndDig, IDC_RADIO5))
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
          if (isGameOn == true)
          {
            //isFirstPlayerTurn
            HWND hwndButton = (HWND)lParam;
            if (GetWindowTextLength(hwndButton) == 0)
            {
              HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
              CHAR szText[500];
              wsprintf(szText, "X");
              SetWindowText(hwndButton, szText);
              CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
              if (isResult(hwndDig, hwndButton, 0))
              {
                wsprintf(szText, "Wygrał X");
                SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                isGameOn = false;
              }
              else if (remis)
              {
                wsprintf(szText, "Remis");
                SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                isGameOn = false;
              }
              if (isGameOn) {
                isComputer(hwndDig, 0);
                CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
                if (isResult2())
                {
                  isGameOn = false;
                  wsprintf(szText, "Wygrał O");
                  SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                }
                else if (remis)
                {
                  wsprintf(szText, "Remis");
                  SetWindowText(hwndStatic2, szText); //Przypisanie go do przycisku
                  isGameOn = false;
                }
              }
              return TRUE;
            }
          }
          return TRUE;

        case IDC_BUTTON1: //klikniecie na naszym przycisku NOWA GRA

          if (isGameOn == false)
          {
            isFree = false;
            iPlace = 0;
            first = true;
            licznik = 0;
            remis = false;
            emptyField = 0;
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