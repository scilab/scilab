/* Copyright INRIA */
#include "C-LAB_Interf.h"
#include "tksci.h"

/* scilab interface */

/* INTERFACE ROUTINE */
typedef void (*voidf)();

extern void LAB_uicontrol();
extern void LAB_TK_DoOneEvent();
extern void LAB_TK_EvalStr();
extern void LAB_TK_EvalFile();
extern void LAB_TK_GetVar();
extern void LAB_TK_SetVar();
extern void LAB_opentk();
extern void LAB_findobj();
extern void LAB_set();
extern void LAB_get();
extern void LAB_figure();
extern void LAB_close();
extern void LAB_uimenu();
extern void LAB_gcf();
extern void LAB_setgvar();
extern void LAB_getgvar();

voidf FuncI[] ={
  LAB_uicontrol,
  LAB_TK_DoOneEvent,
  LAB_TK_EvalStr,
  LAB_TK_EvalFile,
  LAB_TK_GetVar,
  LAB_TK_SetVar,
  LAB_opentk,
  LAB_findobj,
  LAB_set,
  LAB_get,
  LAB_figure,
  LAB_close,
  LAB_uimenu,
  LAB_gcf,
  LAB_setgvar,
  LAB_getgvar
};
     
void C2F(tksciInterf)()
{
  InterfInit();
  if (TK_Started==1)
    (*FuncI[Interf.FuncIndex -1])();
  else 
    Cout("Sorry, TK has not been enabled this the session.\n");

  InterfDone();
}
