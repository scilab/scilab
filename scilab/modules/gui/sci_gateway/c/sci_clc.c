/*------------------------------------------------------------------------*/
/* file: sci_clc.c                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for clc routine                                       */
/*------------------------------------------------------------------------*/

#include "sci_clc.h"
#include "../src/c/wsci/wtext.h"
#include "../src/c/wsci/WinConsole.h"
#include "../src/c/wsci/TextWindows.h"
#include "stack-c.h"

/*-----------------------------------------------------------------------------------*/
int sci_clc( char * fname, unsigned long fname_len )
{
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
}
/*-----------------------------------------------------------------------------------*/