/*------------------------------------------------------------------------*/
/* file: sci_raise_window.h                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for show_window routine                               */
/*------------------------------------------------------------------------*/

#include "sci_raise_window.h"
#ifdef _MSC_VER
#include "../src/c/wsci/wtext.h"
#include "../src/c/wsci/WinConsole.h"
#include "../src/c/wsci/TextWindows.h"
#else
#include "x_ptyxP.h"
#include "x_data.h"
#include "x_error.h"
#include "x_menu.h"
#include "machine.h"
#include "All-extern-x.h"
#endif
#include "gw_gui.h"
#include "../../graphics/includes/WindowList.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_raise_window) _PARAMS(( char * fname, unsigned long fname_len ))
{
#ifdef _MSC_VER
  static int l1, m1, n1;	
  if (IsWindowInterface())
  {
    if (Rhs == 0)
    {
      sciprint(MSG_ERROR80);
    }
    else
    {
      struct BCG *ScilabGC=NULL;
      int num_win=-2;
      CheckLhs(1,1);
      GetRhsVar(1,"i",&m1,&n1,&l1);
      num_win=*istk(l1);
      LhsVar(1)=0;
      ScilabGC = getWindowXgcNumber (num_win);
      if (num_win == -1)
      {
        LPTW lptw=GetTextWinScilab();
        if ( IsIconic(lptw->hWndParent) )
        {
          ShowWindow(lptw->hWndParent,SW_RESTORE);
          ConsoleIsMinimized=FALSE;


        }
        else
        {
          ShowWindow(lptw->hWndParent,SW_MINIMIZE);
          ConsoleIsMinimized=TRUE;
        } 


      }
      else if (ScilabGC != (struct BCG *) 0)
      {
        if ( IsIconic(ScilabGC->hWndParent) )
        {
          ShowWindow(ScilabGC->hWndParent,SW_RESTORE);
        }
        else
        {
          ShowWindow(ScilabGC->hWndParent,SW_MINIMIZE);
          ForceToActiveWindowParent();
        }


      }

    }
  }		
  else
  {
    sciprint(MSG_WARNING29);
  }
  C2F(putlhsvar)();
  return 0;
#else
  Scierror(999,"\nNot yet implemented. \n");	
  return 0;


#endif


}
/*-----------------------------------------------------------------------------------*/
