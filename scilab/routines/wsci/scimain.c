#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

extern int MyWinMain();

int main()
{
  HMODULE x = GetModuleHandle (0);
 
  MyWinMain (x, 0, GetCommandLine (), 1);
  return (0);
}

