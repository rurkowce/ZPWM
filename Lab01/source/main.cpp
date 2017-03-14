#include <Windows.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
 MessageBox(0, "Wybierz liczbe z zakresu 0-40", "Aplikacja", MB_OK);

  int pierwsza = 0;
  int ostatnia = 40;
  int srodek = (pierwsza + ostatnia) / 2;
  char text[100];

  while (pierwsza <= ostatnia) {

    sprintf_s(text, "Czy twoja liczba jest mniejsza od %d?", srodek);
    int iRetKey = MessageBox(0, text, "Aplikacja", MB_YESNO);
      if (iRetKey == IDYES) {
        ostatnia = srodek - 1;
        srodek = (pierwsza + ostatnia) / 2;
    }
      else {
        pierwsza = srodek + 1;
        srodek = (pierwsza + ostatnia) / 2;
      }
  }
  sprintf_s(text, "Twoja liczba to: %d", srodek);
  MessageBox(0, text, "Aplikacja", MB_OK);

  return 0;
}