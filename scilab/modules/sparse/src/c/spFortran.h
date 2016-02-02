/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
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

#ifndef __SPFORTRAN_H__
#define __SPFORTRAN_H__

/*
 *  Routine Renaming
 */
#include "machine.h"
#define sfCreate                C2F(sfcreate)
#define sfStripFills            C2F(sfstripfills)
#define sfDestroy               C2F(sfdestroy)
#define sfClear                 C2F(sfclear)
#define sfGetElement            C2F(sfgetelement)
#define sfGetAdmittance         C2F(sfgetadmittance)
#define sfGetQuad               C2F(sfgetquad)
#define sfGetOnes               C2F(sfgetones)
#define sfAdd1Real              C2F(sfadd1real)
#define sfAdd1Imag              C2F(sfadd1imag)
#define sfAdd1Complex           C2F(sfadd1complex)
#define sfAdd4Real              C2F(sfadd4real)
#define sfAdd4Imag              C2F(sfadd4imag)
#define sfAdd4Complex           C2F(sfadd4complex)
#define sfOrderAndFactor        C2F(sforderandfactor)
#define sfFactor                C2F(sffactor)
#define sfPartition             C2F(sfpartition)
#define sfSolve                 C2F(sfsolve)
#define sfSolveTransposed       C2F(sfsolvetransposed)
#define sfPrint                 C2F(sfprint)
#define sfFileMatrix            C2F(sffilematrix)
#define sfFileVector            C2F(sffilevector)
#define sfFileStats             C2F(sffilestats)
#define sfMNA_Preorder          C2F(sfmna_preorder)
#define sfScale                 C2F(sfscale)
#define sfMultiply              C2F(sfmultiply)
#define sfDeterminant           C2F(sfdeterminant)
#define sfError                 C2F(sferror)
#define sfWhereSingular         C2F(sfwheresingular)
#define sfGetSize               C2F(sfgetsize)
#define sfSetReal               C2F(sfsetreal)
#define sfSetComplex            C2F(sfsetcomplex)
#define sfFillinCount           C2F(sffillincount)
#define sfElementCount          C2F(sfelementcount)
#define sfDeleteRowAndCol       C2F(sfdeleterowandcol)
#define sfPseudoCondition       C2F(sfpseudocondition)
#define sfCondition             C2F(sfcondition)
#define sfNorm                  C2F(sfnorm)
#define sfLargestElement        C2F(sflargestelement)
#define sfRoundoff              C2F(sfroundoff)

/*
 *  MATRIX SPALLOCATION
 *
 *  Allocates and initializes the data structures associated with a matrix.
 *
 *  >>> Returned: [INTEGER]
 *  A pointer to the matrix is returned cast into an integer.  This pointer
 *  is then passed and used by the other matrix routines to refer to a
 *  particular matrix.  If an error occurs, the NULL pointer is returned.
 *
 *  >>> Arguments:
 *  Size  <input>  (long *) [INTEGER]
 *      Size of matrix or estimate of size of matrix if matrix is EXPANDABLE.
 *  Complex  <input>  (int *) [INTEGER or INTEGER*2]
 *      Type of matrix.  If Complex is 0 then the matrix is real, otherwise
 *      the matrix will be complex.  Note that if the routines are not set up
 *      to handle the type of matrix requested, then a spPANIC error will occur.
 *      Further note that if a matrix will be both real and complex, it must
 *      be specified here as being complex.
 *  Error  <output>  (int *) [INTEGER or INTEGER*2]
 *      Returns error flag, needed because function spError() will not work
 *      correctly if spCreate() returns NULL.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 *  spPANIC
 *  Error is cleared in this routine.
 */

long sfCreate(int  *Size, int  *Complex, int  *Error );

/*
 *  MATRIX DEALLOCATION
 *
 *  Deallocates pointers and elements of Matrix.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to the matrix frame which is to be removed from memory.
 */
void sfDestroy( long *Matrix );


/*
 *  STRIP FILL-INS FROM MATRIX
 *
 *  Strips the matrix of all fill-ins.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to the matrix to be stripped.
 */

void sfStripFills( long *Matrix );

/**
 * <long-description>
 *
 * @param Matrix
 */
void sfClear( long *Matrix );


#endif /* __SPFORTRAN_H__ */
