/*------------------------------------------------------------------------*/
/* file: sci_tohome.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for tohome routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_tohome.h"
#ifdef _MSC_VER
#include "../src/c/wsci/TextWindows.h"
#include "../src/c/wsci/WinConsole.h"
#include "../src/c/wsci/wcommon.h"
#include "../src/c/wsci/Errors.h"
#else
#include "x_ptyxP.h"
#include "x_data.h"
#include "x_error.h"
#include "x_menu.h"
#include "machine.h"
#include "All-extern-x.h"
#endif

#include "stack-c.h"
#include "gw_gui.h"
/*-----------------------------------------------------------------------------------*/
extern int IsConsoleMode(void);

/*-----------------------------------------------------------------------------------*/
int sci_tohome( char * fname, unsigned long fname_len )
{
#ifdef _MSC_VER
  LPTW lptw=GetTextWinScilab();

  if ( IsWindowInterface() )
  {
    int nYinc = 0;
    int cy= 0;

    cy = lptw->CursorPos.y * lptw->CharSize.y;
    nYinc =  cy  - lptw->ScrollPos.y;

    lptw->ScrollPos.y += nYinc;
    ScrollWindow (lptw->hWndText, 0, -nYinc, NULL, NULL);

    UpdateWindow (lptw->hWndText);

  }
  else { sciprint(MSG_WARNING31); } 

  C2F(putlhsvar)();
  return 0 ;
#else
  register TScreen *screen = &term->screen;
  static int k=0;
  
  if (!IsConsoleMode())
    {
      /* Screen is filled with empty lines and then cleared */
      for(k=0;k<(Height(screen) - 1)/FontHeight(screen) - 2;k++) /* 2 is the number of rows used by tohome()\r\n */
	{
	  sciprint("\r\n");
	}
      screen->cur_row = screen->topline; /* New cursor position */
      ClearBelow(screen); /* Screen is cleared below cursor */
      ScrollBarDrawThumb(screen->scrollWidget);
    }
  else
    {
      system("clear");
    }
  return 0;
#endif
}
/*-----------------------------------------------------------------------------------*/
