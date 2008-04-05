/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 * desc : interface for show_window routine
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_raise_window)( char * fname, unsigned long fname_len )
{
	  Scierror(999,_("Not yet implemented.\n"));	
	  return 0;
//#ifdef _MSC_VER
//  static int l1, m1, n1;	
//  if (getScilabMode() == SCILAB_STD)
//  {
//    if (Rhs == 0)
//    {
//      sciprint(MSG_ERROR80);
//    }
//    else
//    {
//      struct BCG *ScilabGC=NULL;
//      int num_win=-2;
//      CheckLhs(1,1);
//      GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
//      num_win=*istk(l1);
//      LhsVar(1)=0;
//      ScilabGC = getWindowXgcNumber (num_win);
//      if (num_win == -1)
//      {
//        LPTW lptw=GetTextWinScilab();
//        if ( IsIconic(lptw->hWndParent) )
//        {
//          ShowWindow(lptw->hWndParent,SW_RESTORE);
//          ConsoleIsMinimized=FALSE;
//
//
//        }
//        else
//        {
//          ShowWindow(lptw->hWndParent,SW_MINIMIZE);
//          ConsoleIsMinimized=TRUE;
//        } 
//
//
//      }
//      else if (ScilabGC != (struct BCG *) 0)
//      {
//        if ( IsIconic(ScilabGC->hWndParent) )
//        {
//          ShowWindow(ScilabGC->hWndParent,SW_RESTORE);
//        }
//        else
//        {
//          ShowWindow(ScilabGC->hWndParent,SW_MINIMIZE);
//          ForceToActiveWindowParent();
//        }
//
//
//      }
//
//    }
//  }		
//  else
//  {
//    sciprint(MSG_WARNING29);
//  }
//  C2F(putlhsvar)();
//  return 0;
//#else
//  Scierror(999,"\nNot yet implemented.\n");	
//  return 0;
//
//
//#endif


}
/*--------------------------------------------------------------------------*/
