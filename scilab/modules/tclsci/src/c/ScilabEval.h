/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
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
