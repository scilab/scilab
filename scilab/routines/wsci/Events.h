/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef __EVENTS__
#define __EVENTS__

#include <windows.h>
#include "wcommon.h"

/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp */

#define PRESSED_LEFT 0
#define PRESSED_MIDDLE 1
#define PRESSED_RIGHT 2

#define RELEASED_LEFT -5
#define RELEASED_MIDDLE -4
#define RELEASED_RIGHT -3

#define CLCK_LEFT 3
#define CLCK_MIDDLE 4
#define CLCK_RIGHT 5

#define DBL_CLCK_LEFT 10
#define DBL_CLCK_MIDDLE 11
#define DBL_CLCK_RIGHT 12

#define CTRL_KEY 1000

#define TIMER1LEFTBUTTON 10
#define TIMER2LEFTBUTTON 20

#define TIMER1MIDDLEBUTTON 30
#define TIMER2MIDDLEBUTTON 40

#define TIMER1RIGHTBUTTON 50
#define TIMER2RIGHTBUTTON 60

int GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);

void ON_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
void ON_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
void ON_WM_LBUTTONDBLCLK(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);

void ON_WM_MBUTTONDOWN(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
void ON_WM_MBUTTONUP(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
void ON_WM_MBUTTONDBLCLK(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);

void ON_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
void ON_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
void ON_WM_RBUTTONDBLCLK(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);

void ON_WM_MOVE(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);

#endif /*__EVENTS__*/
/*-----------------------------------------------------------------------------------*/
