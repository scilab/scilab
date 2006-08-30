/*------------------------------------------------------------------------*/
/* file: sci_tohome.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for tohome routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_tohome.h"
#include "../src/c/wsci/TextWindows.h"
#include "../src/c/wsci/WinConsole.h"
#include "../src/c/wsci/wcommon.h"
#include "stack-c.h"
#include "../src/c/wsci/Errors.h"

/*-----------------------------------------------------------------------------------*/
int sci_tohome( char * fname, unsigned long fname_len )
{
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
}
/*-----------------------------------------------------------------------------------*/