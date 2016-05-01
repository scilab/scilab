/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
