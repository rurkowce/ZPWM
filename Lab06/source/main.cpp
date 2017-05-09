#include <Windows.h>
#include <stdio.h>
#include "res.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdio.h>


#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

float fStep = 0.05;
HDC hDC = NULL;
HGLRC hRC = NULL;
GLuint idTexture;
bool isGameOn = false;
bool isFirstPlayerTurn = true;
bool remis = false;
int licznik = 0;
bool isFieldOccupiedByFirstPlayer[3][3];
bool isFieldOccupiedBySecondPlayer[3][3];
char tab[3][3] = { { '0', '1', '2' },
{ '3', '4', '5' },
{ '6', '7', '8' } };

unsigned char* ReadBmpFromFile(char* szFileName, int &riWidth, int &riHeight)
{
  BITMAPFILEHEADER     bfh;
  BITMAPINFOHEADER     bih;

  int                i, j, h, v, lev, l, ls;
  unsigned char*     buff = NULL;

  unsigned char* p_palette = NULL;
  unsigned short n_colors = 0;

  unsigned char* pRGBBuffer=nullptr;

  FILE* hfile=nullptr;
  fopen_s(&hfile, szFileName, "rb");

  if (hfile != NULL)
  {
    fread(&bfh, sizeof(bfh), 1, hfile);
    if (!(bfh.bfType != 0x4d42 || (bfh.bfOffBits < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))))
    {
      fread(&bih, sizeof(bih), 1, hfile);
      v = bih.biWidth;
      h = bih.biHeight;
      lev = bih.biBitCount;

      riWidth = v;
      riHeight = h;
      pRGBBuffer = new unsigned char[riWidth*riHeight * 3]; //Zaalokowanie odpowiedniego buffora obrazu

                                                            //Załaduj Palete barw jesli jest
      if ((lev == 1) || (lev == 4) || (lev == 8))
      {
        n_colors = 1 << lev;
        p_palette = new unsigned char[4 * n_colors];
        fread(p_palette, 4 * n_colors, 1, hfile);
      }

      fseek(hfile, bfh.bfOffBits, SEEK_SET);

      buff = new unsigned char[v * 4];

      switch (lev)
      {
      case 1:
        //Nie obsługiwane
        break;
      case 4:
        //nie Obsługiwane
        break;
      case 8: //Skala szarości
        ls = (v + 3) & 0xFFFFFFFC;
        for (j = (h - 1); j >= 0; j--)
        {
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0; i<v; i++)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
          }
        };
        break;
      case 24:
        //bitmapa RGB
        ls = (v * 3 + 3) & 0xFFFFFFFC;
        for (j = (h - 1); j >= 0; j--)
        {
          //x_fread(hfile,buff,ls);
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0; i<v; i++, l += 3)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 2];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 0];
          };
        };
        break;
      case 32:
        // RGBA bitmap 
        for (j = (h - 1); j >= 0; j--)
        {
          fread(buff, v * 4, 1, hfile);
          for (i = 0, l = 0; i<v; i++, l += 4)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 2];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 0];
          }
        };
        break;
      };
      delete buff;
      if (p_palette) delete p_palette;

    }
  }
  return pRGBBuffer;
}
void DrawCircle(int a, int b);
void DrawCross(int a, int b);
GLint DrawGLScene();
void DrawBoard();


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
  case WM_INITDIALOG:
  {
    int iWidth;
    int iHeight;
    unsigned char* pMyImage = ReadBmpFromFile("..\\res\\kwiatki.bmp", iWidth, iHeight);

    //zmienic styl okienka
    SetWindowLong(hwndDig, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC|WS_OVERLAPPEDWINDOW);

    //stworzyć maszyne stanu
    static PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      16,
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      16,
      0,
      0,
      PFD_MAIN_PLANE,
      0,
      0, 0, 0 };

    //ciag zmiennych

    GLuint PixelFormat;
    hDC = GetDC(hwndDig);
    PixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, PixelFormat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);



      glGenTextures(1, &idTexture);
      glBindTexture(GL_TEXTURE_2D, idTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pMyImage);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      delete[] pMyImage;

    return TRUE;
  }

  case WM_SIZE:
    {
    GLint iWidth = LOWORD(lParam);
    GLint iHeight = HIWORD(lParam);
    if (iHeight == 0) iHeight = 1;
    if (iWidth == 0) iWidth = 1;
      glViewport(0, 0, iWidth, iHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glLoadMatrix()
    gluPerspective(45.0f, iWidth / iHeight, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return TRUE;
    }

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

            if (isResult(hwndDig,hdc))
            {
              HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 125, 0));
              SelectObject(hdc, hMyPen);
              isGameOn = false;
              wsprintf(szText, "Wygrany: Krzyżyk");
              TextOut(hdc, 250, 10, szText, strlen(szText));
              return TRUE;
            }
            if(remis)
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
    fStep = 1.0;
    SetTimer(hwndDig, 1, 2000, NULL);
    return TRUE;
  }

  case WM_TIMER:
    {
    if (wParam == 1)
    {
      fStep = 0.0;
      KillTimer(hwndDig, 1);
    }
    if(wParam==2)
    {
      DrawGLScene();
      DrawBoard();
      glPushMatrix();
      glTranslatef(2.5, -10, 0);
      static float fangle = 0;
      glRotatef(fangle, 0, 1, 1);
      fangle += fStep;
      DrawCircle(0, 0);
      glPopMatrix();
      DrawCross(1.5, 1);
      SwapBuffers(hDC);
    }
    return TRUE;
    }
  /*case WM_PAINT:
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
    */

  case WM_CLOSE:
    DestroyWindow(hwndDig);  //zniszczenie okienka
    PostQuitMessage(0); //umieszczenie w pętli komunikatów polecenia zakonczenia aplikacji
    return TRUE;
  }
  return FALSE;
  }

void DrawCube(int a, int b, int c)
{
  glBindTexture(GL_TEXTURE_2D, idTexture);
  glBegin(GL_QUADS);
  glColor3d(0,0, 1);
  glNormal3d(0, -1, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(0,0,0);
  glTexCoord2f(0.0, 1.0); glVertex3d(0,0,c);
  glTexCoord2f(0.5, 1.0); glVertex3d(a, 0, c);
  glTexCoord2f(0.5, 0.0); glVertex3d(a, 0, 0);

  glColor3d(0, 1, 0);
  glNormal3d(0,-1, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(a, 0, 0);
  glTexCoord2f(0.0, 1.0); glVertex3d(a, 0, c);
  glTexCoord2f(1.0, 1.0); glVertex3d(a, b, c);
  glTexCoord2f(1.0, 0.0); glVertex3d(a, b, 0);

  glColor3d(1, 0, 0); 
  glNormal3d(0, 1, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(a, b, 0);
  glTexCoord2f(0.0, 1.0); glVertex3d(a, b, c);
  glTexCoord2f(1.0, 1.0); glVertex3d(0, b, c);
  glTexCoord2f(1.0, 0.0); glVertex3d(0, b, 0);

  glColor3d(0, 1, 1);
  glNormal3d(-1, 0, 0);
  glTexCoord2f(0.0, 0.0); glVertex3d(0, b, 0);
  glTexCoord2f(0.0, 1.0); glVertex3d(0, b, c);
  glTexCoord2f(1.0, 1.0); glVertex3d(0, 0, c);
  glTexCoord2f(1.0, 0.0); glVertex3d(0, 0, 0);

  glColor3d(1,0,1);
  glNormal3d(0, 0, 1);
  glTexCoord2f(0.0, 0.0); glVertex3d(0, 0, c);
  glTexCoord2f(0.0, 0.1); glVertex3d(0, b, c);
  glTexCoord2f(0.1, 0.1); glVertex3d(a, b, c);
  glTexCoord2f(0.1, 0.0); glVertex3d(a, 0, c);

  glColor3d(1,1,0);
  glNormal3d(0,0,-1);
  glTexCoord2f(0.0, 0.0); glVertex3d(0, 0, 0);
  glTexCoord2f(0.0, 1.0); glVertex3d(a, 0, 0);
  glTexCoord2f(1.0, 1.0); glVertex3d(a, b, 0);
  glTexCoord2f(1.0, 0.0); glVertex3d(0, b, 0);
  glEnd();
}

 void DrawCross(int a,int b)
{
  //przód
   //glScalef(0.4, 0.4, 0.4);
   glPushMatrix();
   glScalef(0.4, 0.4, 0.4);
   glBegin(GL_QUADS);
   glColor3d(1, 0, 0);
   glNormal3d(1, 0, 0);
   glVertex3d(a + 1, b + 8, 0);
   glVertex3d(a + 6, b + 2, 0);
   glVertex3d(a + 8, b + 2, 0);
   glVertex3d(a + 3, b + 8, 0);
   glVertex3d(a + 1, b + 2, 0);
   glVertex3d(a + 3, b + 2, 0);
   glVertex3d(a + 8, b + 8, 0);
   glVertex3d(a + 6, b + 8, 0);
  //tył
   glColor3d(1, 1, 0);
   glNormal3d(1, 1, 0);
   glVertex3d(a + 1, b + 8, 1);
   glVertex3d(a + 6, b + 2, 1);
   glVertex3d(a + 8, b + 2, 1);
   glVertex3d(a + 3, b + 8, 1);
   glVertex3d(a + 1, b + 2, 1);
   glVertex3d(a + 3, b + 2, 1);
   glVertex3d(a + 8, b + 8, 1);
   glVertex3d(a + 6, b + 8, 1);
  //bok lewy góra
   glColor3d(1, 0, 1);
   glNormal3d(1, 0, 1);
   glVertex3d(a + 1, b + 8, 0);
   glVertex3d(a + 1, b + 8, 1);
   glVertex3d(a + 6, b + 2, 1);
   glVertex3d(a + 6, b + 2, 0);
   //bok lewy dół
   glVertex3d(a + 3, b + 8, 0);
   glVertex3d(a + 3, b + 8, 1);
   glVertex3d(a + 8, b + 2, 1);
   glVertex3d(a + 8, b + 2, 0);
   //bok lewy góra
   glColor3d(0, 0, 1);
   glNormal3d(0, 0, 1);
   glVertex3d(a + 1, b + 2, 0);
   glVertex3d(a + 1, b + 2, 1);
   glVertex3d(a + 6, b + 8, 1);
   glVertex3d(a + 6, b + 8, 0);
  //bok lewy dół
   glVertex3d(a + 3, b + 2, 0);
   glVertex3d(a + 3, b + 2, 1);
   glVertex3d(a + 8, b + 8, 1);
   glVertex3d(a + 8, b + 8, 0);
  //góra
   glColor3d(0, 1, 0);
   glNormal3d(0, 1, 0);
   glVertex3d(a + 1, b + 2, 0);
   glVertex3d(a + 1, b + 2, 1);
   glVertex3d(a + 3, b + 2, 1);
   glVertex3d(a + 3, b + 2, 0);
   glVertex3d(a + 6, b + 2, 0);
   glVertex3d(a + 6, b + 2, 1);
   glVertex3d(a + 8, b + 2, 1);
   glVertex3d(a + 8, b + 2, 0);
  //dół
   glVertex3d(a + 1, b + 8, 0);
   glVertex3d(a + 1, b + 8, 1);
   glVertex3d(a + 3, b + 8, 1);
   glVertex3d(a + 3, b + 8, 0);
   glVertex3d(a + 6, b + 8, 0);
   glVertex3d(a + 6, b + 8, 1);
   glVertex3d(a + 8, b + 8, 1);
   glVertex3d(a + 8,b+8, 0);
   glEnd();
   glPopMatrix();

}
void DrawCircle(int a, int b)
{
  //przód
  //glScalef(0.4, 0.4, 0.4);
  glPushMatrix();
  glScalef(0.4, 0.4, 0.4);
  glBegin(GL_QUADS);
  glColor3d(1, 0, 0);
  glNormal3d(1,0, 0);
  glVertex3d(a+1, b + 8, 0);
  glVertex3d(a + 1, b + 2, 0);
  glVertex3d(a + 3, b + 2, 0);
  glVertex3d(a + 3, b + 8, 0);
  glVertex3d(a + 1, b + 2, 0);
  glVertex3d(a + 7, b + 2, 0);
  glVertex3d(a + 7, b + 4, 0);
  glVertex3d(a + 1, b + 4, 0);
  glVertex3d(a + 5, b + 2, 0);
  glVertex3d(a + 7, b + 2, 0);
  glVertex3d(a + 7, b + 8, 0);
  glVertex3d(a + 5, b + 8, 0);
  glVertex3d(a + 1, b + 8, 0);
  glVertex3d(a + 1, b + 6, 0);
  glVertex3d(a + 7, b + 6, 0);
  glVertex3d(a + 7, b + 8, 0);
  //tył
  glColor3d(0, 1, 0);
  glNormal3d(0,1, 0);
  glVertex3d(a + 1, b + 8, 1);
  glVertex3d(a + 1, b + 2, 1);
  glVertex3d(a + 3, b + 2, 1);
  glVertex3d(a + 3, b + 8, 1);
  glVertex3d(a + 1, b + 2, 1);
  glVertex3d(a + 7, b + 2, 1);
  glVertex3d(a + 7, b + 4, 1);
  glVertex3d(a + 1, b + 4, 1);
  glVertex3d(a + 5, b + 2, 1);
  glVertex3d(a + 7, b + 2, 1);
  glVertex3d(a + 7, b + 8, 1);
  glVertex3d(a + 5, b + 8, 1);
  glVertex3d(a + 1, b + 8, 1);
  glVertex3d(a + 1, b + 6, 1);
  glVertex3d(a + 7, b + 6, 1);
  glVertex3d(a + 7, b + 8, 1);
  //bok lewy
  glColor3d(0,0,1);
  glNormal3d(0,0,1);
  glVertex3d(a + 1, b + 8, 0);
  glVertex3d(a + 1, b + 2, 0);
  glVertex3d(a + 1, b + 2, 1);
  glVertex3d(a + 1, b + 8, 1);
  //góra
  glVertex3d(a + 1, b + 2, 0);
  glVertex3d(a + 1, b + 2, 1);
  glVertex3d(a + 7, b + 2, 1);
  glVertex3d(a + 7, b + 2,0);
  //bok prawy
  glVertex3d(a + 7, b + 2, 0);
  glVertex3d(a + 7, b + 2, 1);
  glVertex3d(a + 7, b + 8, 1);
  glVertex3d(a + 7, b + 8, 0);
  //dół
  glVertex3d(a + 1, b + 8, 0);
  glVertex3d(a + 1, b + 8, 1);
  glVertex3d(a + 7, b + 8, 1);
  glVertex3d(a + 7, b + 8, 0);
  //środek
  glColor3d(1, 1, 0);
  glNormal3d(1, 1, 0);
  glVertex3d(a + 3, b + 6, 0);
  glVertex3d(a + 3, b + 6, 1);
  glVertex3d(a + 3, b + 4, 1);
  glVertex3d(a + 3, b + 4, 0);
  glVertex3d(a + 5, b + 6, 0);
  glVertex3d(a + 5, b + 6, 1);
  glVertex3d(a + 5, b + 4, 1);
  glVertex3d(a + 5, b + 4, 0);
  glVertex3d(a + 3, b + 4, 0);
  glVertex3d(a + 3, b + 4, 1);
  glVertex3d(a + 5, b + 4, 1);
  glVertex3d(a + 5, b + 4, 0);
  glVertex3d(a + 3, b + 6, 0);
  glVertex3d(a + 3, b + 6, 1);
  glVertex3d(a + 5, b + 6, 1);
  glVertex3d(a + 5, b+6, 0);

  glEnd();
  glPopMatrix();
}
void DrawBoard()
{
  //os Y

  DrawCube(1,10,1);
  DrawCube(1,-5,1);
  //os X
  DrawCube(10, 1, 1);
  DrawCube(-5, 1, 1);

  glTranslatef(5,5, 0);
  DrawCube(1, 5, 1);
  DrawCube(1,-10,1);
  //X
  DrawCube(-10, 1, 1);
  DrawCube(5, 1, 1);
}

  GLint DrawGLScene()
  {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    glLoadIdentity();
    static GLfloat fAngle = 0.0;
    fAngle += 0.01f;
    glTranslatef(0,0, -30);
    glRotatef(fAngle, 1, 1, 1);
    //glScalef(0.4, 0.4, 0.4);

    glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(10, 0, 0);
    glVertex3d(-10, 0, 0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 10, 0);
    glVertex3d(0, -10, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 10);
    glVertex3d(0, 0, -10);
    glEnd();

    return 1;
  }

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  MSG msg = {};
  SetTimer(hwndMainWindow, 2,50, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }



  /*BOOL bDone = false;
  while (!bDone)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT) bDone = true;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      DrawGLScene();
      DrawBoard();
      glPushMatrix();
      glTranslatef(2.5,-10, 0);
      static float fangle = 0;
      glRotatef(fangle, 0, 1, 1);
      fangle += fStep;
      DrawCircle(0,0);
      glPopMatrix();
      DrawCross(1.5,1);
      SwapBuffers(hDC);
    }
  }
  */
  return 0;
}

