#include <Windows.h>
#include <stdio.h>
#include "res.h"

bool isGameOn = false;
bool isFirstPlayerTurn = true;


INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    
      case EN_CHANGE:
        /*switch (LOWORD(wParam)) {
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
        */
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
            CHAR szText[500];
            if (isFirstPlayerTurn == true) {
              wsprintf(szText, "X");
              HWND hwndRadio1 = GetDlgItem(hwndDig, IDC_RADIO1);
              CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
            }
            else {
              wsprintf(szText, "O");
              HWND hwndRadio1 = GetDlgItem(hwndDig, IDC_RADIO2);
              CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
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
          /*BOOL CheckRadioButton{
            _In_ HWND hDlg,
            _In_ int nIDFirstButton,
            _In_ int nIDLastButton,
            _In_ int nIDFCheckButton,
          };
          */
          if (isFirstPlayerTurn = true) {
            HWND hwndRadio1 = GetDlgItem(hwndDig, IDC_RADIO1);
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
          }
          else {
            HWND hwndRadio1 = GetDlgItem(hwndDig, IDC_RADIO1);
            CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
          }

          isGameOn = true;
          HWND hwndStatic = GetDlgItem(hwndDig, IDC_STATIC1);  //Pobranie uchwytu kontrolki IDC_STATIC1
          CHAR szText[500];
          wsprintf(szText, "Gra siê rozpocze³a");
          SetWindowText(hwndStatic, szText); //Przypisanie go do przycisku
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


/*  case WM_LBUTTONDOWN:
    CHAR szText[200];
    wsprintf(szText, "Kliknales myszka x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDig, szText, TEXT("Klikniecie"), MB_OK);
    return TRUE;
    */

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