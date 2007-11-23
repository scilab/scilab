/*--------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "clrscrPart_nw.h"
#include "gotoxy_nw.h"
/*--------------------------------------------------------------------------*/
void clrscrPart_nw(int nblines)
{
	COORD coord;
	DWORD written;
	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	coord.X = 0;
	coord.Y = info.dwCursorPosition.Y - nblines - 1 ;

	gotoxy_nw(0, coord.Y);

	FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ', info.dwSize.X * (info.dwSize.Y - coord.Y -1), coord, &written);
	gotoxy_nw(1, coord.Y);

}
/*--------------------------------------------------------------------------*/
