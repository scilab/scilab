/* Copyright INRIA */
/* UI generator main routine */
/* Bertrand Guiheneuf, INRIA 1997 */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "TK_uicontrol.h"



void LAB_uimenu()


{
  int NbParam;
  int i;
  char *tmpstr;
  char MyCommand[2000];
  char *StrHandle;
  int Handle;

  Matrix *Mfield;
  Matrix *Mvalue;
  
  Matrix *MOutputHandle;
  double *OutputHandle;

  int FirstField=0;
  int FigureHandle=0;

  int found=0;
  
  
  NbParam = Interf.NbParamIn;


  if ( (NbParam >0) && MatrixIsNumeric(Interf.Param[0]) )
    /* the first parameter is a figure handle */
    {
      FigureHandle = (int)(floor( MatrixGetScalar(Interf.Param[0]) ) );
      FirstField = 1;
    }

  /* Search for a "parent" field */  
  for (i=FirstField; ( (i<NbParam) && !(found) ); i++)
    {
      if (MatrixIsString(Interf.Param[i]))
	{
	  tmpstr=MatrixReadString(Interf.Param[i]);
	  nocase(tmpstr);
	  found = !strcmp(tmpstr,"parent");
	  free(tmpstr);
	}
    }

  if (found)
    {
      if ( !(i<NbParam) )
	{
	  InterfError("uicontrol : you must specify a handle value after \"handle\"\n");
	  return;
	}
      else
	{
	  if (!MatrixIsReal(Interf.Param[i]))
	    {
	      InterfError("uicontrol : you must specify a real value after \"handle\"\n");
	      return;
	    }
	  else
	    {
	      /* OK this time */
	      FigureHandle = (int)MatrixGetScalar(Interf.Param[i]);
	      
	    }
	}
    }

  sprintf(MyCommand, "set MyTmpBertrand [CreateUIMenu %d ];", FigureHandle); 

  Tcl_Eval(TKinterp,MyCommand);
  StrHandle = (char *)Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
  Handle = (int)atoi(StrHandle);

  /* Now let's set all propoerties for the uicontrol */
  for (i=FirstField; i<NbParam; i++)
    {
      Mfield = Interf.Param[i];
      if (++i==NbParam)
	{
	  InterfError("uimenu :The last value is missing \n");
	  return;
	} 

      else Mvalue = Interf.Param[i];


      /* (*UiSet[Style])(Mfield, Mvalue); */
      TK_UiSet(Handle, Mfield, Mvalue);
    }


  MOutputHandle = MatrixCreate(1,1,"real");
  OutputHandle = (double *)MatrixGetPr(MOutputHandle);
  *OutputHandle = Handle;

  ReturnParam(MOutputHandle);



}














