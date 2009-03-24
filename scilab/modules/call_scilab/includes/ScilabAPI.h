/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABAPI__
#define __SCILABAPI__


/**
 * Read a Matrix from the Scilab memory
 * calling sequence 
 *     logic=creadmat('matrixname',m,n,scimat)
 * @example
 *    Amat is a real 2 x 3 scilab matrix 
 *    your subroutine should be as follows: 
 *    subroutine mysubr(...) 
 *    ... 
 * @code
 *    call readmat('Amat',m,n,scimat) 
 *    => m=3 , n=2, and scimat(1)=Amat(1,1) 
 *                      scimat(2)=Amat(2,1) 
 *                      scimat(3)=Amat(3,1) 
 *                      scimat(4)=Amat(1,2) ... 
 *                      scimat(5)=Amat(3,2) 
 *                      scimat(6)=Amat(3,2) 
 * @endcode
 * @param variableName  The name of the variable
 * @param m Number of rows
 * @param n Number of columns
 * @param scimat Matrix entries stored columnwise 
 * @param name_len the length of variableName
 * @return if the operation successed (true) or not (false)
 */
int C2F(readmat)(char *variableName, int *m, int *n, double *scimat, unsigned long name_len);

/*--------------------------------------------------------------------------*/
#endif /* __SCILABAPI__ */
/*--------------------------------------------------------------------------*/

