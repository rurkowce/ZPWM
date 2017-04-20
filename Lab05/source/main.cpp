#include <Windows.h>
#include <stdio.h>
#include "res.h"

bool isGameOn = false;
bool isFirstPlayerTurn = true;
bool remis = false;
int licznik = 0;
//int tab[9] = {0};
char tab[3][3] = { { '0', '1', '2' },
{ '3', '4', '5' },
{ '6', '7', '8' } };
bool isFieldOccupiedByFirstPlayer[3][3];
bool isFieldOccupiedBySecondPlayer[3][3];
HBITMAP hBitmapGameBoard;
HBITMAP hBitmapReset;
HBITMAP hBitmapO;
HBITMAP hBitmapStart;
HBITMAP hBitmapX;
HINSTANCE hInst;

LRESULT CALLBACK ButtonWndProc(
  HWND hwnd,  //handle to window
  UINT uMsg,  //message identifier
  WPARAM wParam, // first message parametr
  LPARAM lParam) //second message parametr
{
  switch(uMsg)
  {
  case WM_PAINT:
  {
    //paint the window client area  HDC hDCBitmap;
    HDC hdc = GetDC(hwnd);
    HDC hDCBitmap;
    hDCBitmap = CreateCompatibleDC(hdc);
    SelectObject(hDCBitmap, hBitmapStart);
    BitBlt(hdc, 0 , 0, 106, 44, hDCBitmap, 0, 0, SRCCOPY);
    DeleteDC(hDCBitmap);
    ReleaseDC(hwnd, hdc);
    return 0;
  }
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    //return CallWindowProc(wpOrgButtonProc, hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

void DrawX(HDC hdc, int x, int y)
{
  /*
	HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
	SelectObject(hdc, hMyPen);
	MoveToEx(hdc, x - 10, y - 10, NULL);
	LineTo(hdc, x + 10, y + 10);
	MoveToEx(hdc, x - 10, y + 10, NULL);
	LineTo(hdc, x + 10, y - 10);
	DeleteObject(hMyPen);
  */
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapX);
  BitBlt(hdc, x, y, 87, 82, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
}

void DrawO(HDC hdc, int x, int y)
{
  /*
	HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
  SelectObject(hdc, hMyPen);
	MoveToEx(hdc, x - 10, y - 10, NULL);
	LineTo(hdc, x - 10, y + 10);
	LineTo(hdc, x - 10, y - 10);
	LineTo(hdc, x + 10, y - 10);
	LineTo(hdc, x + 10, y + 10);
	LineTo(hdc, x - 10, y + 10);
	DeleteObject(hMyPen);
  */
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapO);
  BitBlt(hdc, x, y, 87, 82, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
}

void DrawBoard(HDC hdc)
{
  /*
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
  */
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapGameBoard);
  BitBlt(hdc, 0, 0, 388,249, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
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
/*
int isResult(HWND hwndDig, HDC x)
{
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
  SelectObject(x, hMyPen);
  CHAR szText[500];
  for (int i = 0; i < 3; i++)
  {
    if (((tab[3 * i] == tab[3 * i + 1]) && (tab[3 * i] == tab[3 * i + 2])))
    {
      if (tab[3 * i] == 1)
      {
        wsprintf(szText, "Wygrany: Krzyżyk");
        TextOut(x, 250, 10, szText, strlen(szText));
        isGameOn = false;
        return TRUE;
      }
      else if (tab[3 * i] == 2)
      {
        wsprintf(szText, "Wygrany: Kwadrat");
        TextOut(x, 250, 10, szText, strlen(szText));
        isGameOn = false;
        return TRUE;
      }
    }
  }
  DeleteObject(hMyPen);
  return TRUE;
}
*/

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
          for (int i = 0; i < 300; i++)
            for (int j = 390; j < 550; j++)
            {
              SetPixel(hdc, j, i, GetSysColor(COLOR_MENU));
            }
          //rysowanie siatki gry
          DrawBoard(hdc);
          licznik = 0;
          int a = 0;
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

  case WM_INITDIALOG:
  {
    HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1);  //Pobranie uchwytu kontrolki IDC_BUTTON1
    WNDPROC wpOrgButtonProc = (WNDPROC)SetWindowLong(hwndButton1, -4, (LONG)ButtonWndProc);  //-4 to jest GWL_WNDPROC
    hBitmapGameBoard = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLANSZA));
    hBitmapReset = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RESET));
    hBitmapStart = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_START));
    hBitmapX = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_X));
    hBitmapO = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_O));
    return TRUE;
  }

  case WM_LBUTTONDOWN:
  {
    HDC hdc = GetDC(hwndDig);
    if (isGameOn == true)
    {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if ((x > 0 && x < 388) && (y > 0 && y < 249))
      {
        int filedX = ((x - 0) / 129);
        int filedY = ((y - 0) / 83);

        if (isFieldOccupiedByFirstPlayer[filedX][filedY] == false
          && isFieldOccupiedBySecondPlayer[filedX][filedY] == false)
        {
          x = (filedX * 129)+ (129 / 2); //x=((x-iMinBoardX)/iWidthBoardX)*iWidthBoardX+iWidthBoardX/2
          y = (filedY * 83)+ (83 / 2);

          if (isFirstPlayerTurn == true)
          {
            DrawX(hdc, x-43, y-41);
            isFieldOccupiedByFirstPlayer[filedX][filedY] = true;
            tab[filedX][filedY] = 'X';
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);

            if (isResult(hwndDig, hdc))
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "Wygrany: Krzyżyk");
              TextOut(hdc, 400, 10, szText, strlen(szText));
              return TRUE;
            }
            if (remis)
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "REMIS");
              TextOut(hdc, 400, 10, szText, strlen(szText));
              return TRUE;
            }
          }
          else
          {
            DrawO(hdc, x-43, y-41);
            isFieldOccupiedBySecondPlayer[filedX][filedY] = true;
            tab[filedX][filedY] = 'O';
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

            if (isResult(hwndDig, hdc))
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "Wygrany: Kółko");
              TextOut(hdc, 400, 10, szText, strlen(szText));
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
    BitBlt(hdc, 0, 0, 269, 174, GetDC(0), 0, 0, SRCCOPY); // przezroczyste
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
    DeleteObject(hBitmapGameBoard);
    DeleteObject(hBitmapX);
    DeleteObject(hBitmapO);
    DeleteObject(hBitmapReset);
    DeleteObject(hBitmapStart);
    PostQuitMessage(0); //umieszczenie w pętli komunikatów polecenia zakonczenia aplikacji
    return TRUE;
  }
  return FALSE;
  }




int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
  hInst = hinstance;
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