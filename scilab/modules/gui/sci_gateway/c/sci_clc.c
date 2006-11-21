/*------------------------------------------------------------------------*/
/* file: sci_clc.c                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for clc routine                                       */
/*------------------------------------------------------------------------*/

#include "sci_clc.h"
#ifdef _MSC_VER
#include "../src/c/wsci/wtext.h"
#include "../src/c/wsci/WinConsole.h"
#include "../src/c/wsci/TextWindows.h"
#else

#include "../src/c/xsci/x_ptyxP.h"
#include "../src/c/xsci/x_data.h"
#include "../src/c/xsci/x_error.h"
#include "../src/c/xsci/x_menu.h"
#include "machine.h"
#include "../src/c/xsci/All-extern-x.h"

#endif
#include "stack-c.h"

extern int IsConsoleMode(void);

/*-----------------------------------------------------------------------------------*/
int sci_clc( char * fname, unsigned long fname_len )
{
#ifdef _MSC_VER
  if (Rhs == 0) /* aucun parametre On Efface tout l'ecran */
  {
    if ( IsWindowInterface() )
    {
      LPTW lptw=GetTextWinScilab();
      ClearCommandWindow(lptw,FALSE);
    }
    else
    {
      system("cls");	
    }
  }
  else /* on remonte le curseur en effacant du nombre de lignes indiqué */
  {
    if ( IsWindowInterface() )
    {
      static int l1, m1, n1;
      int NbrLineToRemove=0;

      CheckRhs(1,1);
      CheckLhs(1,1);
      GetRhsVar(1,"i",&m1,&n1,&l1);
      NbrLineToRemove=*istk(l1)+2;
      LhsVar(1)=0;

      if (NbrLineToRemove>0)
      {
        ClearLinesScreenConsole(NbrLineToRemove);
      }	
      else
      {
        sciprint(MSG_ERROR58,NbrLineToRemove);
      }
    }
    else
    {
      sciprint(MSG_WARNING29);
    }
  }
  C2F(putlhsvar)();
  return 0;	

#else
  register TScreen *screen = &term->screen;
  int l1,m1=1,n1=1,NbLines=0;

  if (!IsConsoleMode())
    {
      CheckRhs(0,1);
      if (Rhs==0)
	{
	  ClearScreen(screen); /* All screen is cleared */
	  screen->savedlines = 0; /* Number of lines scrolled */
	  screen->cur_row = screen->topline - 1; /* New cursor position */
	  sciprint("\r\n"); /* To have a blank space above prompt */
	  ScrollBarDrawThumb(screen->scrollWidget);
	}
      else
	{
	  GetRhsVar(1,"i",&m1,&n1,&l1);
	  NbLines = *istk(l1);
	  if (NbLines>0)
	    {
	      if (screen->topline < screen->cur_row - NbLines)
		{
		  screen->savedlines = screen->savedlines - NbLines - 2; /* Number of lines scrolled */
		  if (screen->savedlines < 0)
		    screen->savedlines=0;
		  screen->cur_row = screen->cur_row - NbLines - 2; /* New cursor position */
		  ClearBelow(screen); /* Screen is cleared below cursor */
		  ScrollBarDrawThumb(screen->scrollWidget);
		}
	      else
		{
		  sciprint("\n Not in screen \n");
		}
	    }
	  else
	    {
	      sciprint("\n Error %d invalid number \n",NbLines);
	    }
	}
    }
  else
    {
      sciprint("\n Only in Window Mode \n");
    }
  return 0;
#endif

}
/*-----------------------------------------------------------------------------------*/
