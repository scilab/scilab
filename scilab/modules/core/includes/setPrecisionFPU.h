/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

/**
 * Set the precision of the FPU to double precision (PC=10 / 53bit)
 * This function should not be used in Scilab 5.0
 */
void setFPUToDouble(void);

/**
 * Set the precision of the FPU to extended precision (PC=11 / 64bit)
 * This is the normal precision in Scilab 3 => 5.0
 * We usually use it to revert the FPU precision changed after a JVM call.
 */
void setFPUToExtended(void);
