/* Copyright INRIA */
/* UI generator main routine */
/* Bertrand Guiheneuf, INRIA 1997 */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "TK_uicontrol.h"



void LAB_uicontrol()


{


  
  int NbParam;
  
  int found=0;
  int i;
  
  char *tmpstr;
  int Style=0; /* Default type is pushbutton */
  char *StyleStr=NULL;


  char MyCommand[2000];
  char *StrHandle;
  int Handle;

  Matrix *Mfield;
  Matrix *Mvalue;
  
  Matrix *MOutputHandle;
  double *OutputHandle;

  int FirstField=0;
  int FigureHandle=0;
  
  NbParam = Interf.NbParamIn;


  if ( (NbParam >0) && MatrixIsScalar(Interf.Param[0]) && MatrixIsReal(Interf.Param[0]) )
    /* the first parameter is a figure handle */
    {
      FigureHandle = (int)(floor( MatrixGetScalar(Interf.Param[0]) ) );
      FirstField = 1;
    }

  /* Search for a "Style" field */  
  for (i=FirstField; ( (i<NbParam) && !(found) ); i++)
    {
      if (MatrixIsString(Interf.Param[i]))
	{
	  tmpstr=MatrixReadString(Interf.Param[i]);
	  nocase(tmpstr);
	  found = !strcmp(tmpstr,"style");
	  free(tmpstr);
	}
    }

  if (found)
    {
      if ( !(i<NbParam) )
	{
	  InterfError("uicontrol : you must specify a Style value after \"Style\"\n");
	  return;
	}
      else
	{
	  if (!MatrixIsString(Interf.Param[i]))
	    {
	      InterfError("uicontrol : you must specify a string value after \"Style\"\n");
	      return;
	    }
	  else
	    {
	      /* OK this time, It's a string after "Style" */
	      StyleStr = MatrixReadString(Interf.Param[i]);
	      nocase(StyleStr);
	      
	    }

	}

    }


  if (StyleStr) 
    {
      Style=GetStyle(StyleStr);
      free(StyleStr);
    }
  else Style=0;

  if (Style==-1) 
    {
      InterfError("uicontrol : the specified style does not exist\n");
      return;
    } 



  sprintf(MyCommand, "set MyTmpBertrand [CreateUIControl %d %s];", FigureHandle, UiStyleName[Style]); 

  Tcl_Eval(TKinterp,MyCommand);
  StrHandle = (char*)Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
  Handle = (int)atoi(StrHandle);

  /* Now let's set all propoerties for the uicontrol */
  for (i=FirstField; i<NbParam; i++)
    {
      Mfield = Interf.Param[i];
      if (MatrixIsString(Interf.Param[i]))
	{
	  if (++i==NbParam)
	    {
	      InterfError("uicontrol :The last value is missing \n");
	      return;
	    } 
	
	  else Mvalue = Interf.Param[i];
	}
      else
	{
	  InterfError("uicontrol : incorrect syntax.\n");
	  return;
	}

      /* (*UiSet[Style])(Mfield, Mvalue); */
      TK_UiSet(Handle, Mfield, Mvalue);
  
    }


  MOutputHandle = MatrixCreate(1,1,"real");
  OutputHandle = (double *)MatrixGetPr(MOutputHandle);
  *OutputHandle = Handle;

  ReturnParam(MOutputHandle);



}














