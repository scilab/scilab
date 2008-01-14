/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <Windows.h>
#include <conio.h>
#include "GetCharWithEventsLoop.h"
#include "dynamic_menus.h"

/*--------------------------------------------------------------------------*/ 
int GetCharWithEventsLoop(int interrupt)
{
	int ch = 0;
	while( !_kbhit() && ismenu()==0) 
	{
		Sleep(1);
	}
	ch = _getch ();
	return ch;
}
/*--------------------------------------------------------------------------*/ 