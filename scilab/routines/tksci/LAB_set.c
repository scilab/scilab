/* Copyright INRIA */

#include "C-LAB_Interf.h"
#include "TK_uicontrol.h"
#include "TK_ScilabCallback.h"
#include "tksci.h"


void LAB_set()


{


  Matrix *MHandle;
  int Handle;

  Matrix *Mfield;
  char *field;

  Matrix *Mvalue;
  char *value;

  if (Interf.NbParamIn < 3)
    {
      InterfError("Wrong number of input arguments");
      return;
    }
  MHandle = (Matrix *)Interf.Param[0];
  if (!MatrixIsReal(MHandle) )
    {
      InterfError("Handle must be an integer");
      return;
    }
  Handle = (int)MatrixGetScalar(MHandle);

  Mfield  = (Matrix *)Interf.Param[1];
  if (!MatrixIsString(Mfield) )
    {
      InterfError("Field parameter must be a string");
      return;
    }

  Mvalue  = (Matrix *)Interf.Param[2];
  
  if (TK_UiSet(Handle, Mfield, Mvalue)==-1)
    {
      InterfError("Value parameter type is not correct");
      return;
    }

  
}
