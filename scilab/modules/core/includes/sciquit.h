
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __SCIQUIT__
#define __SCIQUIT__

#include "machine.h"

/**
 * Clean up and end Scilab
 * from fortran
 */
void sciquit(void);

/**
 *  Cleanup (terminate) all the different features/modules of scilab 
 * (GUI, TCL/TK, JVM ...)
 * @return 0 is the end of function is reached
 */
int ExitScilab(void);

#endif /*__SCIQUIT__*/
/*--------------------------------------------------------------------------*/ 
