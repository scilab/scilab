/* Copyright INRIA */

#include "C-LAB_Interf.h"
#include "TK_uicontrol.h"
#include "TK_ScilabCallback.h"
#include "tksci.h"


extern int TK_UiGet(int Handle,Matrix * Mfield,Matrix ** Mvalue);
void LAB_get()
{
  Matrix *MHandle, *Mfield, *Mvalue;
  int Handle;

  if (Interf.NbParamIn < 2)
    {
      InterfError("Wrong number of input arguments");
      return;
    }

  if (Interf.NbParamOut > 1 )
    {
      InterfError("Wrong number of output arguments");
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
  
  if (TK_UiGet(Handle, Mfield, &Mvalue)==-1)
    {
      InterfError("get()");
      return;
    }
  else {    ReturnParam(Mvalue); }
}
