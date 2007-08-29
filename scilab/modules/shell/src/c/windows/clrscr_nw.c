/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include "clrscr_nw.h"
#include "gotoxy_nw.h"
/*-----------------------------------------------------------------------------------*/
void clrscr_nw(void)
{
	COORD coord;
	DWORD written;
	CONSOLE_SCREEN_BUFFER_INFO info;
	coord.X = 0;
	coord.Y = 0;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ', info.dwSize.X * info.dwSize.Y, coord, &written);
	gotoxy_nw(1, 1);
}
/*-----------------------------------------------------------------------------------*/
