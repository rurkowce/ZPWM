#include <Windows.h>
#include <stdio.h>
#include "res.h"

bool isGameOn = false;
bool isFirstPlayerTurn = true;
bool remis = false;
int licznik = 0;
bool isFieldOccupiedByFirstPlayer[3][3];
bool isFieldOccupiedBySecondPlayer[3][3];
char tab[3][3] = { { '0', '1', '2' },
{ '3', '4', '5' },
{ '6', '7', '8' } };


void DrawX(HDC hdc, int x, int y)
{
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
  SelectObject(hdc, hMyPen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  LineTo(hdc, x + 10, y + 10);
  MoveToEx(hdc, x - 10, y + 10, NULL);
  LineTo(hdc, x + 10, y - 10);
  DeleteObject(hMyPen);
}

void DrawY(HDC hdc, int x, int y)
{
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
  SelectObject(hdc, hMyPen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  LineTo(hdc, x - 10, y + 10);
  LineTo(hdc, x - 10, y - 10);
  LineTo(hdc, x + 10, y - 10);
  LineTo(hdc, x + 10, y + 10);
  LineTo(hdc, x - 10, y + 10);
  DeleteObject(hMyPen);
}

void DrawBoard(HDC hdc)
{
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
  SelectObject(hdc, hMyPen);
  MoveToEx(hdc, 60, 30, NULL);
  LineTo(hdc, 60, 180);
  MoveToEx(hdc, 110, 30, NULL);
  LineTo(hdc, 110, 180);
  MoveToEx(hdc, 10, 80, NULL);
  LineTo(hdc, 160, 80);
  MoveToEx(hdc, 10, 130, NULL);
  LineTo(hdc, 160, 130);
  DeleteObject(hMyPen);
}

bool isResult(HWND hwndDlg, HDC xx)
{
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
  SelectObject(xx, hMyPen);
  if (tab[0][0] == tab[0][1] && tab[0][0] == tab[0][2] ||
    tab[1][0] == tab[1][1] && tab[1][0] == tab[1][2] ||
    tab[2][0] == tab[2][1] && tab[2][0] == tab[2][2] ||
    tab[0][0] == tab[1][0] && tab[0][0] == tab[2][0] ||
    tab[0][0] == tab[1][1] && tab[0][0] == tab[2][2] ||
    tab[0][2] == tab[1][1] && tab[0][2] == tab[2][0] ||
    tab[0][1] == tab[1][1] && tab[0][1] == tab[2][1] ||
    tab[0][2] == tab[1][2] && tab[0][2] == tab[2][2])
  {
    return TRUE;
  }
  else
  {
    licznik++;
    if (licznik == 9) {
      remis = true;
    }
    return false;
  }
}

INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1);  //Pobranie uchwytu kontrolki IDC_BUTTON1
  CHAR szText[500];
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:  //zdarzenie klikniecia na UI
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1: //klikniecie na naszym przycisku NOWA GRA
      {
        if (isGameOn == false)
        {
          //czyszczenie pola gry 
          HDC hdc = GetDC(hwndDig);
          for (int i = 0; i < 190; i++)
            for (int j = 0; j < 500; j++)
            {
              SetPixel(hdc, j, i, GetSysColor(COLOR_MENU));
            }

          //rysowanie siatki gry
          DrawBoard(hdc);
          int a = 0;
          licznik = 0;
          remis = false;
          for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
              tab[i][j] = a++;
          if (isFirstPlayerTurn == true)
          {
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
          }
          else
          {
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
          }

          ReleaseDC(hwndDig, hdc);

          for (int i = 0; i < 3; i++)
          {
            for (int j = 0; j < 3; j++)
            {
              isFieldOccupiedByFirstPlayer[i][j] = false;
              isFieldOccupiedBySecondPlayer[i][j] = false;
            }
            wsprintf(szText, "Stop");
            SetWindowText(hwndButton1, szText); //Przypisanie go do przycisku
            isGameOn = true;
            //CreateBrush
            TextOut(hdc, 0, 0, szText, strlen(szText));
          }
        }
        else
        {
          wsprintf(szText, "Start");
          SetWindowText(hwndButton1, szText); //Przypisanie go do przycisku
          isGameOn = false;
        }
        return TRUE;
      }
      }
      return FALSE;
    }

  case WM_LBUTTONDOWN:
  {
    HDC hdc = GetDC(hwndDig);
    if (isGameOn == true)
    {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if ((x > 10 && x < 160) && (y > 30 && y < 180))
      {
        int filedX = ((x - 10) / 50);
        int filedY = ((y - 30) / 50);

        if (isFieldOccupiedByFirstPlayer[filedX][filedY] == false
          && isFieldOccupiedBySecondPlayer[filedX][filedY] == false)
        {
          x = (filedX * 50) + 10 + 50 / 2; //x=((x-iMinBoardX)/iWidthBoardX)*iWidthBoardX+iWidthBoardX/2
          y = (filedY * 50) + 30 + 50 / 2;

          if (isFirstPlayerTurn == true)
          {
            DrawX(hdc, x, y);
            isFieldOccupiedByFirstPlayer[filedX][filedY] = true;
            tab[filedX][filedY] = 'X';
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);

            if (isResult(hwndDig, hdc))
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "Wygrany: Krzyżyk");
              TextOut(hdc, 250, 10, szText, strlen(szText));
              return TRUE;
            }
            if (remis)
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "REMIS");
              TextOut(hdc, 250, 10, szText, strlen(szText));
              return TRUE;
            }
          }
          else
          {
            DrawY(hdc, x, y);
            isFieldOccupiedBySecondPlayer[filedX][filedY] = true;
            tab[filedX][filedY] = 'O';
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
            if (isResult(hwndDig, hdc))
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "Wygrany: Kwadrat");
              TextOut(hdc, 250, 10, szText, strlen(szText));
              return TRUE;
            }
          }
          ReleaseDC(hwndDig, hdc);
          isFirstPlayerTurn = !isFirstPlayerTurn;
        }
      }
    }
    return TRUE;
  }

  case WM_PAINT:
  {
    HDC hdc = GetDC(hwndDig);
    //SelectObject(hdc, hMyPen);
    DrawBoard(hdc);
    //DrawAllBoards(hdc);
    for (int iFieldX = 0; iFieldX < 3; iFieldX++)
    {
      for (int iFieldY = 0; iFieldY < 3; iFieldY++)
      {
        if (isFieldOccupiedByFirstPlayer[iFieldX][iFieldY] == true)
        {
          //DrawX(hdc,iFieldX,iFieldY);
        }
        if (isFieldOccupiedBySecondPlayer[iFieldX][iFieldY] == true)
        {
          //DrawO(hdc,iFieldX,iFieldY);
        }
      }
    }
    ReleaseDC(hwndDig, hdc);
    return DefWindowProc(hwndDig, uMsg, wParam, lParam);
  }

  case WM_CLOSE:
    DestroyWindow(hwndDig);  //zniszczenie okienka
    PostQuitMessage(0); //umieszczenie w pętli komunikatów polecenia zakonczenia aplikacji
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