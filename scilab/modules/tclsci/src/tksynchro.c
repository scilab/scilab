/* Copyright INRIA */

#include <math.h>
#include <stdio.h>

#include "../machine.h"

#ifdef WITH_TK
#include "../tclsci/TCL_Global.h"
#endif



void C2F(tksynchro)(l)
     int *l;
{
#ifdef WITH_TK
  char str[128];


  if (TK_Started) {
    int RET;

    if(TCLinterp != NULL){
      RET = Tcl_Eval(TCLinterp,"set isscipadinterp [interp exists scipad]");
      if (RET==TCL_ERROR){
	Scierror(999,"Error : tksynchro %s \r\n",TCLinterp->result);
      }
      if(strcmp((char*) Tcl_GetVar(TCLinterp,"isscipadinterp", TCL_GLOBAL_ONLY),"1")==0)
	{
	  sprintf(str,"scipad eval {set sciprompt %d}",*l);
	  RET=Tcl_Eval(TCLinterp,str);
	  
	  if (RET==TCL_ERROR)
	    {
	      Scierror(999,"Error : tksynchro %s \r\n",TCLinterp->result);
	    }
	}
    }
  }
#endif
}
