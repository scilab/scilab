/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef __EVENTS__
#define __EVENTS__

#include <windows.h>
#include "wcommon.h"

/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp */


#define CLCK_LEFT 10
#define CLCK_MIDDLE 11
#define CLCK_RIGHT 12

#define DBL_CLCK_LEFT 10
#define DBL_CLCK_MIDDLE 11
#define DBL_CLCK_RIGHT 12


#define CTRL_DBL_CLCK_LEFT 1010
#define CTRL_DBL_CLCK_MIDDLE 1011
#define CTRL_DBL_CLCK_RIGHT 1012


int GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);


#endif /*__EVENTS__*/
/*-----------------------------------------------------------------------------------*/
