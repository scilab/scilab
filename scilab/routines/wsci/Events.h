/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef __EVENTS__
#define __EVENTS__

#include <windows.h>
#include "../machine.h"
#include "wcommon.h"

typedef struct but{  int win, x, y, ibutton, motion, release;}But;

But SciClickInfo; /* for xclick and xclick_any */

int GetEventKeyboardAndMouse (UINT message, WPARAM wParam, LPARAM lParam);
int PushClickQueue (int win,int x,int y,int ibut,int motion,int release);
int CheckClickQueue (integer *win,integer *x, integer *y, integer *ibut);
int ClearClickQueue (int win);
void set_wait_click(val);
int scig_click_handler_none (int win,int x,int y,int ibut,int motion,int release);
int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release);
Scig_click_handler set_scig_click_handler (Scig_click_handler f);
void reset_scig_click_handler (void);
void scig_deletegwin_handler_none (int win);
void scig_deletegwin_handler_sci (int win);
Scig_deletegwin_handler set_scig_deletegwin_handler (Scig_deletegwin_handler f);
void reset_scig_deletegwin_handler ();
void C2F(seteventhandler)(int *win_num,char *name,int *ierr);



#endif /*__EVENTS__*/
/*-----------------------------------------------------------------------------------*/