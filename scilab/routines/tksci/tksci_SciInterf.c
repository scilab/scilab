/* Copyright INRIA */
#include "C-LAB_Interf.h"
#include "tksci.h"

/* scilab interface */

/* INTERFACE ROUTINE */
typedef void (*voidf)();
extern void Cout(char *str);
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
/*extern void LAB_gcf();*/
extern void LAB_setgvar();
extern void LAB_getgvar();
extern void sci_tk_activate(void);

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
  LAB_get,
  LAB_setgvar,
  LAB_getgvar
};
     
void C2F(tksciInterf)()
{
  static int first =0;
  InterfInit();
  if (TK_Started != 1 )
    {
      if ( first == 0) 
	{
	  sci_tk_activate();first++;
	  if ( TK_Started != 1 ) 
	    {
	      Cout("You have started Scilab in a mode in which TK not initialized.\n");
	      Cout("I've tried initialization but it failed\n");
	      return ;
	    }
	}
    }

  (*FuncI[Interf.FuncIndex -1])();
  InterfDone();

}
