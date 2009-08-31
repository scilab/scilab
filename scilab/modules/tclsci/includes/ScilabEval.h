/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABEVAL__
#define __SCILABEVAL__

#include "dynlib_tclsci.h"

/**
 * @TODO add comment
 *
 * @param clientData    
 * @param theinterp 
 * @param objc  
 * @param argv  
 * @return <ReturnValue>
 */
TCLSCI_IMPEXP  int TCL_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,CONST char ** argv);

#endif /* __SCILABEVAL__ */
/*--------------------------------------------------------------------------*/ 
