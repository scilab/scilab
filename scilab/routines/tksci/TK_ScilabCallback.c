/* Copyright INRIA */
#include "../machine.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"


int TK_EvalScilabCmd(clientData, theinterp, objc, argv)
     ClientData clientData;
     Tcl_Interp *theinterp;
     int objc;
     char **argv;


{
int stl1, stl2, myerr;
char *mycmd;
  /* printf("%s\n", argv[1]); */

StoreCommand(argv[1]);  


/* free(mycmd); */
Tcl_SetResult(theinterp,NULL,NULL);

return TCL_OK;

}
