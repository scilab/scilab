/**
 * INRIA 2007
 * @author Sylvestre Ledru <sylvestre.ledru@inria.fr>
 */
#ifndef __SCILABEVAL_H__
#define __SCILABEVAL_H__

#include "TCL_Global.h"

/**
 * @TODO add comment
 *
 * @param clientData    
 * @param theinterp 
 * @param objc  
 * @param argv  
 * @return <ReturnValue>
 */
int TCL_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,CONST char ** argv);

#endif /* __SCILABEVAL_H__ */
