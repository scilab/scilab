/* Copyright INRIA */
/* UI generator main routine */
/* Bertrand Guiheneuf, INRIA 1997 */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "TK_uicontrol.h"



void LAB_close()


{


  int NbParam;
   
  char MyCommand[2000];
  
  

  int FigureHandle;
  
  
  NbParam = Interf.NbParamIn;
  
  if ( (NbParam >0) && MatrixIsNumeric(Interf.Param[0]) )
    /* the first parameter is a figure handle */
    {
      FigureHandle = (int)(floor( MatrixGetScalar(Interf.Param[0]) ) );
      sprintf(MyCommand, "DestroyFigure %d;", FigureHandle);
      
    } else 
      sprintf(MyCommand, "DestroyFigure 0;"); 
      

  Tcl_Eval(TKinterp,MyCommand);

}














