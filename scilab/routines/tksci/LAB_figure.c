/* Copyright INRIA */
/* UI generator main routine */
/* Bertrand Guiheneuf, INRIA 1997 */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "TK_uicontrol.h"



void LAB_figure()


{
  int NbParam;
  int i;
  char MyCommand[2000];
  char *StrHandle;
  int Handle=0;

  Matrix *Mfield;
  Matrix *Mvalue;
  Matrix *MOutputHandle;

  double *OutputHandle;

  int FigureHandle=0;
  int FirstField=0;
  
  NbParam = Interf.NbParamIn;
  
  if ( (NbParam >0) && MatrixIsNumeric(Interf.Param[0]) )
    /* the first parameter is a figure handle */
    {
      FigureHandle = (int)(floor( MatrixGetScalar(Interf.Param[0]) ) );
      FirstField = 1;
      sprintf(MyCommand, "set MyTmpBertrand [FigureSelect %d];", FigureHandle);
      Tcl_Eval(TKinterp,MyCommand);
      StrHandle = Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
      Handle = (int)atoi(StrHandle);

    } else {
      /* creation of a figure */
      sprintf(MyCommand, "set MyTmpBertrand [CreateFigure 0];"); 
      Tcl_Eval(TKinterp,MyCommand);
      StrHandle = Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
      Handle = (int)atoi(StrHandle);
    }


  /* Now let's set all properties for the uicontrol */
  for (i=FirstField; i<NbParam; i++)
    {
      Mfield = Interf.Param[i];
      if (++i==NbParam)
	{
	  InterfError("figure :The last value is missing \n");
	  return;
	} 

      else Mvalue = Interf.Param[i];
    

    
      TK_UiSet(Handle, Mfield, Mvalue);
    }


  MOutputHandle = MatrixCreate(1,1,"real");
  OutputHandle = (double *)MatrixGetPr(MOutputHandle);
  *OutputHandle = Handle;

  ReturnParam(MOutputHandle);



}














