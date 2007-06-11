
#include <Windows.h>
#include "wcommon.h"

extern TW textwin;

void SendChar(char c)
{
	SendMessage (textwin.hWndText, WM_CHAR, c, 1L);
}
