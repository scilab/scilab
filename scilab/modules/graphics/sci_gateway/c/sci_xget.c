/*------------------------------------------------------------------------*/
/* file: sci_xset.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xget.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "XsetXgetParameters.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "ObjectStructure.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "CurrentObjectsManagement.h"

/*-----------------------------------------------------------------------------------*/
int sci_xget(char *fname,unsigned long fname_len)
{
  integer flagx=0,x1[10],x2=0, m1,n1,l1,m2,n2,l2,l3,v = 0,i ;
  double dv = 0.0;
  int one = 1 ;
  BOOL keyFound = FALSE ;

  SciWin();
  if ( Rhs <= 0 )
  {
    int zero = 0 ;
    sci_demo(fname,"xsetm();",&zero);
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
    i = 105;v=m1*n1;
    strncpy(C2F(cha1).buf,cstk(l1),v);
    C2F(msgs)(&i,&v);
    x2=0;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&x2,&x2,&l3);
    LhsVar(1)=Rhs+1;
    return 0;
  }

  if (Rhs == 2) {
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); 
    CheckScalar(2,m2,n2);  
    flagx = (integer) *stk(l2); }
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

    CreateVar(Rhs+1,"d",&nbRow,&nbCol,&l3) ;

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
  /* NG beg */
  else if ( strcmp(cstk(l1),"old_style") == 0) 
  {
    x2=1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&x2,&l3);
    *stk(l3 ) = 0 ;    
    LhsVar(1)=Rhs+1;          
  }   /* NG end*/
  else 
  {
    int i2;
    sciPointObj *psubwin = sciGetCurrentSubWin();
    x2=0;
    if(strcmp(cstk(l1),"font")==0){
      x1[0] = sciGetFontStyle(psubwin);
      x1[1] = sciGetFontDeciWidth(psubwin)/100;
      x2 = 2;
    }
    else if(strcmp(cstk(l1),"font size")==0){
      x1[0] = sciGetFontDeciWidth(psubwin)/100;
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
    else
    {
      C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);
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
  return 0;
}
/*-----------------------------------------------------------------------------------*/
