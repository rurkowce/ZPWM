#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")
#define PI 3.141592

float fC = 261.626;
float fD = 293.700;
float fD2 = 587.400;   // jako R
float fE = 329.628;
float fF = 349.228;
float fG = 391.995;
float fA = 440.000;   ///małe a
float fP = 220.000;   /// duże A jako P
float fH = 493.883;
float fGIS = 415.300;   // jako X
float fCIS = 544.400;  //jako Y
float fFIS = 370.000;  //jako Z
const int size_imperialny = 18;
const int size_barka = 100;

BYTE* pBufferForAudio = 0;
WAVEFORMATEX pcmWaveFormat;
float S = pcmWaveFormat.nAvgBytesPerSec;
void Song(char *tabNote, double *tabTime, BYTE* pBufferForAudio, int size);

char note_imperialny[size_imperialny] = { 'E','E','E','C','G','E','C','G','E','B','B','B','C','G','E','C','G','E' };
double time_imperialny[size_imperialny] = { 8,8,8,4,2,8,4,2,8,8,8,8,4,2,8,4,2,8, };

char note_barka[size_barka] = { 'Z', 'E', 'Z', 'G', 'Z', 'E', 'D', 'D', 'E', 'Z', 'G', 'G', 'G', 'G', 'G', 'Z', 'E', 'E', 'P', 'D', 'E', 'Z', 'Z','Z', 'G', 'E', 'D', 'D',
                                'D','H','H','H','Y','R','Y','H','A','A','G','Z','G','G','G','A','H','A','G',
                                'Z','Z','D','D','H','H','H','Y','R','Y','H','A','A','G','Z','G','G','E','Z','G','Z','E','D'};
double time_barka[size_barka] = { 42,6,6,6,6,6,18,36,12,6,18,30,6,6,6,6,18,30,6,12,6,18,30,6,12,6,18,36,
                                 18,18,24,6,6,6,6,6,18,36,12,6,18,24,6,6,6,6,6,18,36,12,6,18,24,6,6,6,6,6,18,36,12,6,18,24,
                                 6,6,6,6,6,36, };

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv);

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

        CHAR szText[500];   // tablica dla EditBox
        CHAR lText[500];  //tablica dla licznika
        int iTextLength = GetWindowTextLength(hwndEditBox); //Pobranie dlugosci tekstu
        GetWindowText(hwndEditBox, szText, iTextLength + 1); //Pobranie tekstu
        wsprintf(szText, "Przycisk", LOWORD(lParam), HIWORD(lParam));
        MessageBox(hwndDig, szText, TEXT("Werdykt"), MB_OK);
        return TRUE;

      }
    }
    return FALSE;

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

  pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
  pcmWaveFormat.nChannels = 1;
  pcmWaveFormat.nSamplesPerSec = 44100L;  // L wartość typu LONG
  pcmWaveFormat.wBitsPerSample = 8;
  pcmWaveFormat.nAvgBytesPerSec = pcmWaveFormat.nSamplesPerSec*pcmWaveFormat.wBitsPerSample / 8;
  pcmWaveFormat.nBlockAlign = 1;
  pcmWaveFormat.cbSize = 0;

  MMRESULT mmResult;
  HWAVEOUT hwo = 0;
  UINT devId;

  for (devId = 0; devId<waveOutGetNumDevs(); devId++)
  {
    mmResult = waveOutOpen(&hwo, devId, &pcmWaveFormat, 0, 0, CALLBACK_NULL);
    if (mmResult == MMSYSERR_NOERROR)break;
  }
  if (mmResult != MMSYSERR_NOERROR) //Gdy nie udało sie odtworzyć karty dźwiękowej
  {
    MessageBox(hwndMainWindow, TEXT("Nie znaleziono karty"), TEXT("ERROR"), MB_OK);
    return mmResult;
  }

  BYTE* pBufferForAudio = new BYTE[65 * pcmWaveFormat.nAvgBytesPerSec];   //4 - ile sekund


  //int i = 0;

  /*

  for(;i<0.5* pcmWaveFormat.nAvgBytesPerSec;i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fC;
  pBufferForAudio[i] = 128*sin(2*PI*f*t)+128;
  }
  for (; i<1* pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fD;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  for (; i<1.5* pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fE;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  for (; i<2* pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fF;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  for (; i<2.5* pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fG;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  for (; i<3* pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fA;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  for (; i<3.5* pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  //float f = f1 + (f2 - f1)*t;
  float f = fB;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  */



  WAVEHDR whdr;
  ZeroMemory(&whdr, sizeof(WAVEHDR));
  whdr.lpData = reinterpret_cast<LPSTR>(pBufferForAudio);
  whdr.dwBufferLength = 0 * pcmWaveFormat.nAvgBytesPerSec;
  mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));

  while ((whdr.dwFlags & WHDR_DONE) != WHDR_DONE)Sleep(100); //czekanie
  mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  /*
  for (int i = 0; i<2 * pcmWaveFormat.nAvgBytesPerSec; i++)
  {
  // f = f - 0.01;
  float t = i / (float)pcmWaveFormat.nSamplesPerSec;
  float f = f2 + (f1 - f2)*t;
  pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128;
  }
  */

  //Marsz imperialny
  /*
  whdr.dwBufferLength = 10 * pcmWaveFormat.nAvgBytesPerSec;
  {
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fB, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fB, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fB, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
  i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
  }
  */

  whdr.dwBufferLength = 65 * pcmWaveFormat.nAvgBytesPerSec;
    //Song(note_imperialny, time_imperialny, pBufferForAudio, size_imperialny);
    Song(note_barka, time_barka, pBufferForAudio, size_barka);
    mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
    mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
    while ((whdr.dwFlags & WHDR_DONE) != WHDR_DONE)Sleep(100);
    mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  mmResult = waveOutClose(hwo);
  delete[] pBufferForAudio;
  return 0;
}


void Song(char *tabNote, double *tabTime, BYTE* pBufferForAudio, int size)

{
  int i = 0;
  for (int j = 0; j < size; j++)
  {
    switch (tabNote[j])
    {
    case 'E':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'C':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'G':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'H':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fH, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'D':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'F':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'A':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'X':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGIS, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'Y':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCIS, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'Z':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFIS, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'P':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
      break;
    case 'R':
      i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD2, pcmWaveFormat.nSamplesPerSec);
      break;
    default:
      break;
    }
  }
}

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv)
{
  int i;
  for (i = iStart; i < iStart + iDuration - 200; i++)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128 * sin(2 * PI*fNote*t) + 128;
  }
  for (; i < iStart + iDuration; ++i)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128;
  }
  return i;
}

//tak ma wyglądać
//char snotes = "E E E C G E C G E B B B C G E C G E";
//char sTime = "16 16 16 8 4 16 8 4 16 16 16 16 8 4 16 8 4 16";
