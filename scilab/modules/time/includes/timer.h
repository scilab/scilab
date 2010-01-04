/*--------------------------------------------------------------------------*/
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA 
* Copyright (C) DIGITEO - 2009 - Allan CORNET 
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
