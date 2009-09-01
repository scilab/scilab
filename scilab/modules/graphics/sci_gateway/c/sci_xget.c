/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xset.c                                                       */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "XsetXgetParameters.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "Format.h"
#include "Scierror.h"
#include "ObjectSelection.h"
#include "localization.h"
#include "returnProperty.h"
/*--------------------------------------------------------------------------*/
int C2F(xgetg)( char * str, char * str1, int * len,int  lx0,int lx1);
/*--------------------------------------------------------------------------*/
int sci_xget(char *fname,unsigned long fname_len)
{
  int flagx=0,x1[10],x2=0, m1,n1,l1,m2,n2,l2,l3,i ;
  int one = 1 ;
  BOOL keyFound = FALSE ;

  if ( Rhs <= 0 )
  {
    sci_demo(fname, fname_len);
    return 0;
  }

  CheckRhs(1,2);
  CheckLhs(0,1);

  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

  /* check if key is valid */
  for (i=0; i < NUMSETFONC ; i++)
  {
    if ( strcmp(cstk(l1),KeyTab_[i]) == 0 )
    {
      keyFound = TRUE ;
      break ;
    }
  }

  if ( !keyFound )
  {
    Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, cstk(l1));
    return -1;
  }

  if (Rhs == 2) {
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); 
    CheckScalar(2,m2,n2);  
    flagx = (int)  *stk(l2); }
  else
    flagx = 0;

  if ( strcmp(cstk(l1),"fpf") == 0 || strcmp(cstk(l1),"auto clear") == 0) 
  {
    int bufl;
    /*     special case for global variables set */
    C2F(xgetg)( cstk(l1),C2F(cha1).buf,&bufl,m1,bsiz);
    CreateVar(Rhs+1,STRING_DATATYPE,&bufl,&one,&l3);
    strncpy(cstk(l3),C2F(cha1).buf,bufl);
    LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
    return 0;
  }
  else if ( strcmp(cstk(l1),"colormap") == 0) 
  {
    /*     special case for colormap : must allocate space */
    int nbRow = 0 ;
    int nbCol = 3 ;
    sciPointObj * curFig = sciGetCurrentFigure() ;

    nbRow = sciGetNumColors( curFig ) ;
    if ( nbRow == 0 ) { nbCol = 0 ; }

    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&nbRow,&nbCol,&l3);

    sciGetColormap( curFig, stk(l3) ) ;

    LhsVar(1)=Rhs+1;
  }
  else if ( strcmp(cstk(l1),"mark") == 0)
  {
    int i2;
    sciPointObj * subwin = sciGetCurrentSubWin();
    x1[0] = sciGetMarkStyle(subwin);
    x1[1] = sciGetMarkSize(subwin);
    x2=2;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&x2,&l3);
    for (i2 = 0 ; i2 < x2 ; ++i2)
		*stk(l3 + i2 ) = (double) x1[i2];      
    LhsVar(1)=Rhs+1;
  }
  else if ( strcmp(cstk(l1),"mark size") == 0)
  {
    int i2;
    sciPointObj * subwin = sciGetCurrentSubWin();
    x1[0] = x1[1] = sciGetMarkSize(subwin);

    x1[0]=x1[1];
    x2=1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&x2,&l3);
    for (i2 = 0 ; i2 < x2 ; ++i2) *stk(l3 + i2 ) = (double) x1[i2];      
    LhsVar(1)=Rhs+1;
  }
  else if ( strcmp(cstk(l1),"line style") == 0)
  {
    sciPointObj * subwin = sciGetCurrentSubWin();
    x1[0] = sciGetLineStyle(subwin);
  
    x2=1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&x2,&l3);
    *stk(l3 ) = (double) x1[0];      
    LhsVar(1)=Rhs+1;
  } 
  else if ( strcmp(cstk(l1),"old_style") == 0) 
  {
    x2=1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&x2,&l3);
    *stk(l3 ) = 0 ;    
    LhsVar(1)=Rhs+1;          
  }
  else if(strcmp(cstk(l1),"clipping") == 0)
  {
    double * clipArea = sciGetClipping(sciGetCurrentSubWin());
    sciReturnRowVector(clipArea, 4);
    LhsVar(1)=Rhs+1;
  }
  else 
  {
    int i2;
    sciPointObj *psubwin = sciGetCurrentSubWin();
    x2=0;
    if(strcmp(cstk(l1),"font")==0){
      x1[0] = (int) sciGetFontStyle(psubwin);
      x1[1] = (int) sciGetFontSize(psubwin);
      x2 = 2;
    }
    else if(strcmp(cstk(l1),"font size")==0){
      x1[0] = (int) sciGetFontSize(psubwin);
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"dashes")==0){
      x1[0] = sciGetLineStyle(psubwin);
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"hidden3d")==0){
      x1[0] = pSUBWIN_FEATURE(psubwin)->hiddencolor;
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"window")==0 || strcmp(cstk(l1),"figure") == 0)
    {
      x1[0] = sciGetNum(sciGetCurrentFigure());
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"thickness") == 0)
    {
      x1[0] = (int) sciGetLineWidth(sciGetCurrentSubWin());
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"wdim") == 0)
    {
      x1[0] = sciGetWindowWidth(sciGetCurrentFigure());
      x1[1] = sciGetWindowHeight(sciGetCurrentFigure());
      x2 = 2;
    }
    else if(strcmp(cstk(l1),"wpos") == 0)
    {
      sciGetScreenPosition(sciGetCurrentFigure(), &(x1[0]), &(x1[1]));
      x2 = 2;
    }
    else if(strcmp(cstk(l1),"wpdim") == 0)
    {
      x1[0] = sciGetWindowWidth(sciGetCurrentFigure());
      x1[1] = sciGetWindowHeight(sciGetCurrentFigure());
      x2 = 2;
    }
    else if(strcmp(cstk(l1),"viewport") == 0)
    {
      sciGetViewport(sciGetCurrentFigure(), x1);
      x2 = 2;
    }
    else if(strcmp(cstk(l1),"alufunction") == 0)
    {
      x1[0] = sciGetXorMode(sciGetCurrentFigure());
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"background") == 0)
    {
      x1[0] = sciGetBackground(sciGetCurrentSubWin());
      x2 = 1;
    }
    else if(   strcmp(cstk(l1),"color") == 0
            || strcmp(cstk(l1),"foreground") == 0
            || strcmp(cstk(l1),"pattern") == 0)
    {
      x1[0] = sciGetForeground(sciGetCurrentSubWin());
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"lastpattern") == 0)
    {
      x1[0] = sciGetNumColors(sciGetCurrentFigure());
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"line mode") == 0)
    {
      if (sciGetIsLine(sciGetCurrentSubWin()))
      {
        x1[0] = 1;
      }
      else
      {
        x1[0] = 0;
      }
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"pixmap") == 0)
    {
      if (sciGetPixmapMode(sciGetCurrentFigure()))
      {
        x1[0] = 1;
      }
      else
      {
        x1[0] = 0;
      }
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"white") == 0)
    {
      x1[0] = sciGetNumColors(sciGetCurrentFigure()) + 2;
      x2 = 1;
    }
    else if(strcmp(cstk(l1),"wresize") == 0)
    {
      x1[0] = sciGetResize(sciGetCurrentFigure());
      x2 = 1;
    }
    else if( strcmp(cstk(l1),"clipgrf") == 0 )
    {
      /* special treatement for xset("cligrf") */
      if (sciGetIsClipping(sciGetCurrentSubWin()) == 0)
      {
        x1[0] = 1;
      }
      else
      {
        x1[0] = 0;
      }
      x2 = 1;
    }
    else if( strcmp(cstk(l1),"clipoff") == 0 )
    {
      /* special treatement for xset("cligrf") */
      if (sciGetIsClipping(sciGetCurrentSubWin()) == -1)
      {
        x1[0] = 1;
      }
      else
      {
        x1[0] = 0;
      }
      x2 = 1;
    }
    else
    {
      Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, cstk(l1));
			return -1;
    }
    if (x2 > 0) {
      CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&x2,&l3);
      for (i2 = 0 ; i2 < x2 ; ++i2) *stk(l3 + i2 ) = (double) x1[i2];      
    }
    else {
      x2=0;
      CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&x2,&x2,&l3);
    }
    LhsVar(1)=Rhs+1;
  }
	C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(xgetg)( char * str, char * str1, int * len,int  lx0,int lx1)
{
  if ( strcmp(str,"fpf") == 0) 
  {
    strncpy(str1,getFPF(),32);
    *len= (int) strlen(str1);
  }
  else if ( strcmp(str,"auto clear")==0) 
  {
    int autoclear;
    sciPointObj * subwin = sciGetFirstTypedSelectedSon( sciGetCurrentFigure(), SCI_SUBWIN ) ;
    autoclear = !(sciGetAddPlot(subwin));
    if (autoclear == 1) 
    {
      strncpy(str1,"on",2);
      *len=2;
    }
    else 
    {
      strncpy(str1,"off",3);
      *len=3;
    }
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
