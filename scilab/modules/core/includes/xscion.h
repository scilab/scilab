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
#include "machine.h"
/*--------------------------------------------------------------------------*/ 

/**
 * Is Scilab launched in the standard mode ?
 * @param i the result of the test (1 if Scilab is in the standard mode)
 * @return 0
 */
int C2F(xscion) (int *i);

/**
 * Is Scilab launched in the standard mode ?
 * @return 1 or 0 (1 if Scilab is in the standard mode)
 */
int getINXscilab(void);
/*--------------------------------------------------------------------------*/ 
