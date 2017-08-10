/*--------------------------------------------------------------------------*/
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#ifndef __TIMER_H__
#define __TIMER_H__

#include "machine.h" /* C2F */

/**
* Returns the CPU time since the preceding call to "C2F(timer)"
* used to call from fortran
* uses scilab_timer
* @param etime
* @return 0
*/
int C2F(timer)(double *etime);

/**
* Returns the CPU time since the preceding call to "scilab_timer"
* @return elapsed time
*/
double scilab_timer(void);

#endif /* __TIMER_H__ */
/*--------------------------------------------------------------------------*/
