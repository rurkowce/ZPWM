#include "main.h"
#include <videoInput.h>

videoInput VI;
unsigned char *g_pRGBOriginalSample;
unsigned char *g_pRGBProcesedSample;
int g_iWidth = 320;
int g_iHeight = 240;

unsigned char *g_pRGBLogoSample;
int g_iLogoWidth;
int g_iLogoHeight;
int g_iLogoX;
int g_iLogoY;

//Put Logo - Store File

unsigned char* ReadBmpFromFile(char* szFileName,int &riWidth, int &riHeight)
{
	BITMAPFILEHEADER     bfh;
	BITMAPINFOHEADER     bih;

	int                i,j,h,v,lev,l,ls;
	unsigned char*     buff = NULL;  

  unsigned char* p_palette = NULL;
  unsigned short n_colors = 0;

  unsigned char* pRGBBuffer;

  FILE* hfile = fopen(szFileName,"rb");

	if(hfile!=NULL)
	{
    fread(&bfh,sizeof(bfh),1,hfile);
		if(!(bfh.bfType != 0x4d42 || (bfh.bfOffBits < (sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)))) )
		{
      fread(&bih,sizeof(bih),1,hfile);
			v   = bih.biWidth;
			h   = bih.biHeight;
			lev = bih.biBitCount;
			
      riWidth = v;
      riHeight = h;
      pRGBBuffer = new unsigned char [riWidth*riHeight*3]; //Zaalokowanie odpowiedniego buffora obrazu
      
      //Za³aduj Palete barw jesli jest
      if((lev==1)||(lev==4)||(lev==8))
      {
        n_colors = 1<<lev;
        p_palette = new unsigned char[4*n_colors];
        fread(p_palette,4*n_colors,1,hfile);
      }

      fseek(hfile,bfh.bfOffBits,SEEK_SET);

			buff = new unsigned char[v*4];

			switch(lev)
			{
				case 1:
				//Nie obs³ugiwane
				break;
        case 4:
        //nie Obs³ugiwane
        break;
        case 8: //Skala szaroœci
        ls = (v+3)&0xFFFFFFFC;
				for(j=(h-1);j>=0;j--)
				{
          fread(buff,ls,1,hfile);
          for(i=0,l=0;i<v;i++) 
          {
            pRGBBuffer[((j*riWidth)+i)*3+2] = p_palette[(buff[i]<<2)+2];//R
            pRGBBuffer[((j*riWidth)+i)*3+1] = p_palette[(buff[i]<<2)+1];//G
            pRGBBuffer[((j*riWidth)+i)*3+0] = p_palette[(buff[i]<<2)+0];//B
          }
				};
				break;
				case 24:
				//bitmapa RGB
				ls = (v*3+3)&0xFFFFFFFC;
				for(j=(h-1);j>=0;j--)
				{
					//x_fread(hfile,buff,ls);
          fread(buff,ls,1,hfile);
					for(i=0,l=0;i<v;i++,l+=3) 
					{
						pRGBBuffer[((j*riWidth)+i)*3+0] = buff[l+0];
						pRGBBuffer[((j*riWidth)+i)*3+1] = buff[l+1];
						pRGBBuffer[((j*riWidth)+i)*3+2] = buff[l+2];
					};
				};
				break;
				case 32:
			  // RGBA bitmap 
				for(j=(h-1);j>=0;j--)
				{
          fread(buff,v*4,1,hfile);
					for(i=0,l=0;i<v;i++,l+=4) 
					{
						pRGBBuffer[((j*riWidth)+i)*3+0] = buff[l+0];
						pRGBBuffer[((j*riWidth)+i)*3+1] = buff[l+1];
						pRGBBuffer[((j*riWidth)+i)*3+2] = buff[l+2];
					}
				};
				break;
			};
			delete buff; 
   if(p_palette) delete p_palette;

		}
	}
	return pRGBBuffer;
}

unsigned char* ReadPpmFromFile(char* szFileName,int &riWidth, int &riHeight)
{
  FILE* hfile = fopen(szFileName,"rb");
  int i,j,k,fb;
  int v[3];
  unsigned char buff[10];
  unsigned char* pRGBBuffer;

  if(hfile!=NULL)
  {
    //Odczytanie nag³ówka
    fread(buff,3,1,hfile);
    if((buff[0]=='P')&&(buff[1]=='6')&&(isspace(buff[2])))
    {
      for(i=0;i<3;i++)
      {
        fread(&buff[0],1,1,hfile);
        if(buff[0]=='#') 
        {
          while((buff[0]!=13)&&(buff[0]!=10)) fread(&buff[0],1,1,hfile);
          fread(&buff[0],1,1,hfile);
        }
        k=0;
        do
        {
          k++;
          fread(&buff[k],1,1,hfile);
        }
        while(!isspace(buff[k]));
        buff[k+1] = 0;
        v[i] = atoi((char*)buff);
      }
      v[2] /= 256; //Pobranie dynamiki obrazu 1 lub 2 bajty na próbkê
      
      //Inicjacja obrazu
      if((v[0]*3)%4!=0)
      {
        fb = 4-(((v[0])*3)%4);
      }
      else
      {
        fb = 0;
      }
      riWidth = v[0]+fb;
      riHeight = v[1];
      pRGBBuffer = new unsigned char [riWidth*riHeight*3]; //Zaalokowanie odpowiedniego buffora obrazu
   
      // Wczytanie sk³adowych
      if(v[2]==0)
      {
        for(i=0;i<v[1];i++)
        {
          for(j=0;j<v[0];j++)
          {
            fread(&pRGBBuffer[((riHeight-i-1)*(riWidth)+j)*3+2],1,1,hfile);
            fread(&pRGBBuffer[((riHeight-i-1)*(riWidth)+j)*3+1],1,1,hfile);
            fread(&pRGBBuffer[((riHeight-i-1)*(riWidth)+j)*3+0],1,1,hfile);
          }
          
          /*
          for(j=0;j<fb;j++)
          {
            pTmp[j*3+0] = 0;
            pTmp[j*3+1] = 0;
            pTmp[j*3+2] = 0;
          }
          pTmp+=fb*3;//*/
        }
      }
      else
      {
        //16 bit samples - Nie wspierane
        /*
        for(i=0;i<img->cmp[0]->dy;i++)
          for(j=0;j<img->cmp[0]->dx;j++)
          {
            x_fread(hfile,&buff,6);
            img->cmp[0]->pel[i][j] = buff[0]<<8 & buff[1];
            img->cmp[1]->pel[i][j] = buff[2]<<8 & buff[3];
            img->cmp[2]->pel[i][j] = buff[4]<<8 & buff[5];
          }
          //*/
        delete pRGBBuffer;
        return NULL;
      }
  }
  return pRGBBuffer;
 }
 return NULL;
}

void WriteBmpToFile(char* szFileName,unsigned char* pRGBSamples, int iWidth, int iHeight)
{
  BITMAPFILEHEADER     bfh;
  BITMAPINFOHEADER     bih;
  unsigned char*       buff;
  int                  i,j,l;
  unsigned int         ls,fb;//w

 if ((iWidth)%4!=0) fb=4-(((iWidth)*3)%4);
 else						    fb=0;

 FILE* hfile = fopen(szFileName,"wb");

 if(hfile!=NULL && pRGBSamples!=NULL)
  {
   bfh.bfType          = 0x4d42;
   bfh.bfSize          = ((3*(iWidth)+fb)*(iHeight))+sizeof(bih);
   bfh.bfReserved1     = 0;
   bfh.bfReserved2     = 0;
   bfh.bfOffBits       = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

   bih.biSize          = sizeof(BITMAPINFOHEADER);
   bih.biWidth         = iWidth;  
   bih.biHeight        = iHeight;
   bih.biPlanes        = 1;
   bih.biBitCount      = 24;
   bih.biCompression   = BI_RGB;
   bih.biSizeImage     = 0;
   bih.biXPelsPerMeter = 0;
   bih.biYPelsPerMeter = 0;
   bih.biClrUsed       = 0;
   bih.biClrImportant  = 0;

   fwrite(&bfh,sizeof(bfh),1,hfile);
   fwrite(&bih,sizeof(bih),1,hfile);

   ls = ((iWidth)*3+3)&0xFFFFFFFC;
   buff = new unsigned char[ls];
   for(j=0;j<iHeight;j++)
   {
     for(i=0,l=0;i<iWidth;i++,l+=3)
     {
       buff[l+0] = pRGBSamples[((j*iWidth)+i)*3+0];
       buff[l+1] = pRGBSamples[((j*iWidth)+i)*3+1];
       buff[l+2] = pRGBSamples[((j*iWidth)+i)*3+2];
	   }
     fwrite(buff,ls,1,hfile);
   }
   delete buff; 
 }
}

void WritePpmToFile(char* szFileName,unsigned char* pRGBSamples, int iWidth, int iHeight)
{
  int wrb;
  unsigned char* buff;
  int i,j,k;
  char str[256];
  
  FILE *hfile = fopen(szFileName,"wb");

  if(hfile!=NULL && pRGBSamples!=NULL)
  {
    sprintf(str,"P6\n%d %d\n255\n",iWidth,iHeight);
    fwrite(str,strlen(str),1,hfile);
    for(i=0;i<iHeight;i++)
    {
      for(j=0;j<iWidth;j++) 
      {
        fwrite(&pRGBSamples[((iHeight-i-1)*(iWidth)+j)*3+2],1,1,hfile);
        fwrite(&pRGBSamples[((iHeight-i-1)*(iWidth)+j)*3+1],1,1,hfile);
        fwrite(&pRGBSamples[((iHeight-i-1)*(iWidth)+j)*3+0],1,1,hfile);
      };
    };
  };
}

void DoSomeThingWithSample(unsigned char* pRGBSrcSample,unsigned char* pRGBDsrSample,int iWidth, int iHeight,unsigned char* pRGBLogoSample,int iLogoX, int iLogoY, int iLogoWidth, int iLogoHeight)
{
  for(int y=0;y<iHeight;y++) //Pêtla po wszystkich wierszach obrazu
  {
    for(int x=0;x<iWidth;x++) //Pêtla po wszystkich kolumnach obrazu
    {
      pRGBDsrSample[(y*iWidth+x)*3+0] = pRGBSrcSample[(y*iWidth+x)*3+0]; //Przepisanie s³adowej B
      pRGBDsrSample[(y*iWidth+x)*3+1] = pRGBSrcSample[(y*iWidth+x)*3+1]; //Przepisanie s³adowej G
      pRGBDsrSample[(y*iWidth+x)*3+2] = pRGBSrcSample[(y*iWidth+x)*3+2]; //Przepisanie s³adowej R
    }
  }

  //Kopiowanie logo na obraz
  for(int y=0;y<iLogoHeight;y++)
  {
    for(int x=0;x<iLogoWidth;x++)
    {
      //Jeœli kolor logo jest ciemniejszy ni¿ pewien prób narysuj go na obrazie - w przeciwnym przypadku traktuj jako przeŸroczysty
      if((pRGBLogoSample[((y*iLogoWidth)+x)*3+0]<150)
        &&(pRGBLogoSample[((y*iLogoWidth)+x)*3+1]<150)
        &&(pRGBLogoSample[((y*iLogoWidth)+x)*3+2]<150))
      {
        pRGBDsrSample[((y+iLogoY)*iWidth+(x+iLogoX))*3+0] = pRGBLogoSample[((y*iLogoWidth)+x)*3+0];
        pRGBDsrSample[((y+iLogoY)*iWidth+(x+iLogoX))*3+1] = pRGBLogoSample[((y*iLogoWidth)+x)*3+1];
        pRGBDsrSample[((y+iLogoY)*iWidth+(x+iLogoX))*3+2] = pRGBLogoSample[((y*iLogoWidth)+x)*3+2];
      }
    }
  }
}

void xInitCamera(int iDevice, int iWidth, int iHeight)
{
  int width,height,size;
  VI.setupDevice(iDevice,iWidth,iHeight); // 320 x 240 resolution 

  width = VI.getWidth(iDevice);
  height = VI.getHeight(iDevice);
  size = VI.getSize(iDevice); // size to initialize buffer
  //VI.setUseCallback(true);
}

void xGetFrame(unsigned char* pRGBSample)
{
  int device;
  device = 0;
  if(VI.isFrameNew(device))
  {
    VI.getPixels(device,pRGBSample, false, false);
  }
}

void xDisplayBmpOnWindow(HWND hWnd,int iX, int iY, unsigned char* pRGBSample, int iWidth, int iHeight)
{
  HDC hDC = GetDC(hWnd);
  HDC hDCofBmp = CreateCompatibleDC(hDC);

  HBITMAP hBmp = CreateCompatibleBitmap(hDC,iWidth,iHeight);

  SelectObject(hDCofBmp,hBmp);

  BITMAPINFOHEADER biBmpInfoHeader;
  
  biBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
  biBmpInfoHeader.biWidth = iWidth;
  biBmpInfoHeader.biHeight = iHeight;
  biBmpInfoHeader.biPlanes = 1;
  biBmpInfoHeader.biBitCount = 24;//32
  biBmpInfoHeader.biCompression = BI_RGB;
  biBmpInfoHeader.biSizeImage = 0;
  biBmpInfoHeader.biXPelsPerMeter = 0;
  biBmpInfoHeader.biYPelsPerMeter = 0;
  biBmpInfoHeader.biClrUsed = 0;
  biBmpInfoHeader.biClrImportant = 0;

  SetDIBits(hDCofBmp,hBmp,0,iWidth*iHeight,pRGBSample,(BITMAPINFO*)&biBmpInfoHeader,DIB_RGB_COLORS);

  BitBlt(hDC,iX,iY,iWidth,iHeight,hDCofBmp,0,0,SRCCOPY);

  SelectObject(hDCofBmp,0);
  DeleteObject(hBmp);
  DeleteDC(hDCofBmp);

  ReleaseDC(hWnd,hDC);
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HWND hWndButton;
  //HWND hWndAbout;
  switch (message) 
  { 
  case WM_CREATE:
    hWndButton = CreateWindow(TEXT("BUTTON"),TEXT("Save Image"),BS_FLAT | WS_VISIBLE | WS_CHILD,660,40,120,30,hwnd,(HMENU)LAMB_SAVE_BUTTON,GetModuleHandle(NULL),NULL);
    
    xInitCamera(0,g_iWidth,g_iHeight); //Aktywacja pierwszej kamery do pobierania obrazu o rozdzielczoœci 320x240

    g_pRGBOriginalSample = new unsigned char [g_iWidth*g_iHeight*3]; //Allokacja buffora pamieci na originalne próbki obrazu 
    g_pRGBProcesedSample = new unsigned char [g_iWidth*g_iHeight*3]; //Allokacja buffora pamieci na przetworzone próbki obrazu 

    //g_pRGBLogoSample = ReadPpmFromFile("logo.ppm",g_iLogoWidth, g_iLogoHeight); //Wczyt obrazu z pliku
    g_pRGBLogoSample = ReadBmpFromFile("logo.bmp",g_iLogoWidth, g_iLogoHeight); //Wczyt obrazu z pliku

    g_iLogoX = 10;
    g_iLogoY = 10;

    SetTimer(hwnd,LAMB_ID_TIMER_GET_FRAME,40,NULL); //Ustawienie minutnika na co 40 milisekund

    break;
  case WM_PAINT:     
    xDisplayBmpOnWindow(hwnd,0,0,g_pRGBOriginalSample,g_iWidth,g_iHeight); //Narysowanie naszego buffora próbek obrazu na okienku
    xDisplayBmpOnWindow(hwnd,g_iWidth,0,g_pRGBProcesedSample,g_iWidth,g_iHeight); //Narysowanie naszego buffora próbek obrazu na okienku
    break;
  case WM_TIMER:
    switch(wParam)
    {
    case LAMB_ID_TIMER_GET_FRAME:
      xGetFrame(g_pRGBOriginalSample);  //Pobranie 1 ramki obrazu z kamery 
      DoSomeThingWithSample(g_pRGBOriginalSample,g_pRGBProcesedSample,g_iWidth,g_iHeight,g_pRGBLogoSample,g_iLogoX,g_iLogoY,g_iLogoWidth,g_iLogoHeight); //Wywo³anie procedury przetwarzaj¹cej obraz
      xDisplayBmpOnWindow(hwnd,0,0,g_pRGBOriginalSample,g_iWidth,g_iHeight); //Narysowanie naszego buffora próbek obrazu na okienku
      xDisplayBmpOnWindow(hwnd,g_iWidth,0,g_pRGBProcesedSample,g_iWidth,g_iHeight); //Narysowanie naszego buffora próbek obrazu na okienku
      break;
    }
    break;
  case WM_COMMAND:
    if(HIWORD(wParam)==0)
    {
      switch(LOWORD(wParam))
      {
      case LAMB_SAVE_BUTTON:
        //g_bIsCalibrating = true;
        //WritePpmToFile("Capture.ppm",g_pRGBProcesedSample,g_iWidth,g_iHeight);
        WriteBmpToFile("Capture.bmp",g_pRGBProcesedSample,g_iWidth,g_iHeight);
        break;
      }
    }
    break;
  case WM_DESTROY:
    //DeleteObject(hBitmap);

    delete g_pRGBOriginalSample; //Deallokacja buffora pamieci na originalne próbki obrazu 
    delete g_pRGBProcesedSample; //Deallokacja buffora pamieci na przetworzone próbki obrazu 
    delete g_pRGBLogoSample; //Deallokacje buffora pamieci na obraz logo

    PostQuitMessage(0);
    break;
  }

  return DefWindowProc (hwnd, message, wParam, lParam);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  //Definicja klasy okna
  WNDCLASS wc;
  wc.style = CS_HREDRAW | CS_VREDRAW; 
  wc.lpfnWndProc = (WNDPROC)WndProc; 
  wc.cbClsExtra = 0; 
  wc.cbWndExtra = 0; 
  wc.hInstance = hInstance; 
  wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(LAMB_MAIN_ICON)); 
  wc.hCursor = LoadCursor(NULL, IDC_HAND); 
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
  wc.lpszMenuName = NULL; 
  wc.lpszClassName = LAMB_APP_CLASS_NAME; 

  //Rejestracja klasy okna
  if ( !RegisterClass( &wc ) ) return( FALSE );

  // Tworzenie g³ównego okna aplikacji 
  HWND hWnd = CreateWindow( 
    LAMB_APP_CLASS_NAME,
    LAMB_APP_WINDOW_NAME,
    WS_OVERLAPPEDWINDOW,
    0,
    0,
    g_iWidth*3,
    g_iHeight,
    NULL,
    NULL,
    hInstance, 
    NULL);

  // Sprawdzenie czy okienko siê utworzy³o
  if ( hWnd == NULL ) return( FALSE );

  // Pokazanie okinka na ekranie
  ShowWindow(hWnd,iCmdShow);

  MSG msg;

  // G³ówna pêtla komunikatów
  while( GetMessage( &msg, NULL, 0, 0) )
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  //MessageBox(0,TEXT("Hejka"),TEXT("Informacja"),MB_OK);
  return 0;
}
