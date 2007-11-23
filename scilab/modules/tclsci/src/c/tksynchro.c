/*--------------------------------------------------------------------------*/
/* Copyright INRIA */
/*--------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include "machine.h"
#include "TCL_Global.h"
#include "tksynchro.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(tksynchro)( int *l)
{
  char str[128];

  if (TK_Started) 
  {
    int RET;

    if(TCLinterp != NULL)
	{
      RET = Tcl_Eval(TCLinterp,"set isscipadinterp [interp exists scipad]");
      if (RET==TCL_ERROR)
	  {
		  Scierror(999,_("Error : tksynchro %s.\n"),TCLinterp->result);
      }
      if(strcmp((char*) Tcl_GetVar(TCLinterp,"isscipadinterp", TCL_GLOBAL_ONLY),"1")==0)
	  {
		sprintf(str,"scipad eval {set sciprompt %d}",*l);
		RET=Tcl_Eval(TCLinterp,str);
	  
		if (RET==TCL_ERROR)
	    {
		  Scierror(999,_("Error : tksynchro %s.\n"),TCLinterp->result);
	    }
	  }
    }
  }
}
/*--------------------------------------------------------------------------*/
