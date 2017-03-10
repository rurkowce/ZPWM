#include <Windows.h>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
  int iRetKey = MessageBox(0, "Podoba ci sie moja pierwsza aplikacja", "Aplikacja", MB_YESNO);
  if (iRetKey == IDYES)
  {
    MessageBox(0, "To wspaniale", "Aplikacja", MB_OK);
  }
  else
  {
    MessageBox(0, "A co ci siê nie podoba?", "Aplikacja", MB_OK | MB_ICONQUESTION);
  }
  return 0;
}