/* Copyright INRIA */
#include "C-LAB_Interf.h"
#include "TK_uicontrol.h"
#include "TK_ScilabCallback.h"
#include "tksci.h"
#include <stdio.h>

#if WIN32
extern void sciprint (char *fmt,...);
#endif

void LAB_findobj()
{
  Matrix *Mfield;
  char *field;

  Matrix *Mvalue;
  char *value;

  Matrix *MOutputHandle;
  double *OutputHandle;

  char MyCommand[2000];
  char *StrHandle;
  int Handle;

  if (Interf.NbParamIn ==  2) /* Correction Bug 790 */
	{
		Mfield  = (Matrix *)Interf.Param[0];
		field = MatrixReadString(Mfield);
		nocase(field);

		Mvalue  = (Matrix *)Interf.Param[1];
		value = MatrixReadString(Mvalue);
		nocase(value);
		sprintf(MyCommand, "set MyTmpBertrand [FindObj \"%s\" \"%s\"];", field, value);
  
		Tcl_Eval(TKinterp,MyCommand);
		StrHandle = (char*)Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
		Handle = (int)atoi(StrHandle);
		if (Handle == -1)  /* object not found */ MOutputHandle = (Matrix *)MatrixCreate(0,0,"real");
		else
		{
			/* objet found */
			MOutputHandle = (Matrix *)MatrixCreate(1,1,"real");
			OutputHandle = (double *)MatrixGetPr(MOutputHandle);
			*OutputHandle = (double)Handle;
		}
	}
  else
	{
		sciprint("findobj recquires 2 parameters\n");
		MOutputHandle = (Matrix *)MatrixCreate(0,0,"real");
	}
  ReturnParam(MOutputHandle);
}
