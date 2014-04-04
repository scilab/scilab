/*
 *  SPARSE FORTRAN MODULE
 *
 *  Author:                     Advising professor:
 *     Kenneth S. Kundert           Alberto Sangiovanni-Vincentelli
 *     UC Berkeley
 *
 *  This module contains routines that interface Sparse1.3 to a calling
 *  program written in fortran.  Almost every externally available Sparse1.3
 *  routine has a counterpart defined in this file, with the name the
 *  same except the `sp' prefix is changed to `sf'.  The spADD_ELEMENT
 *  and spADD_QUAD macros are also replaced with the sfAdd1 and sfAdd4
 *  functions defined in this file.
 *
 *  To ease porting this file to different operating systems, the names of
 *  the functions can be easily redefined (search for `Routine Renaming').
 *  A simple example of a FORTRAN program that calls Sparse is included in
 *  this file (search for Example).  When interfacing to a FORTRAN program,
 *  the ARRAY_OFFSET option should be set to NO (see spConfig.h).
 *
 *  DISCLAIMER:
 *  These interface routines were written by a C programmer who has little
 *  experience with FORTRAN.  The routines have had minimal testing.
 *  Any interface between two languages is going to have portability
 *  problems, this one is no exception.
 *
 *
 *  >>> User accessible functions contained in this file:
 *  sfCreate()
 *  sfDestroy()
 *  sfStripFills()
 *  sfClear()
 *  sfGetElement()
 *  sfGetAdmittance()
 *  sfGetQuad()
 *  sfGetOnes()
 *  sfAdd1Real()
 *  sfAdd1Imag()
 *  sfAdd1Complex()
 *  sfAdd4Real()
 *  sfAdd4Imag()
 *  sfAdd4Complex()
 *  sfOrderAndFactor()
 *  sfFactor()
 *  sfPartition()
 *  sfSolve()
 *  sfSolveTransposed()
 *  sfPrint()
 *  sfFileMatrix()
 *  sfFileVector()
 *  sfFileStats()
 *  sfMNA_Preorder()
 *  sfScale()
 *  sfMultiply()
 *  sfTransMultiply()
 *  sfDeterminant()
 *  sfError()
 *  sfWhereSingular()
 *  sfGetSize()
 *  sfSetReal()
 *  sfSetComplex()
 *  sfFillinCount()
 *  sfElementCount()
 *  sfDeleteRowAndCol()
 *  sfPseudoCondition()
 *  sfCondition()
 *  sfNorm()
 *  sfLargestElement()
 *  sfRoundoff()
 */

/*
 *  FORTRAN -- C COMPATIBILITY
 *
 *  Fortran and C data types correspond in the following way:
 *  -- C --     -- FORTRAN --
 *  int         INTEGER*4 or INTEGER*2 (machine dependent, usually int*4)
 *  long        INTEGER*4
 *  float       REAL
 *  double      DOUBLE PRECISION (used by default in preference to float)
 *
 *  The complex number format used by Sparse is compatible with that
 *  used by FORTRAN.  C pointers are passed to FORTRAN as longs, they should
 *  not be used in any way in FORTRAN.
 */



/*
 *  Revision and copyright information.
 *
 *  Copyright (c) 1985,86,87,88
 *  by Kenneth S. Kundert and the University of California.
 *
 *  Permission to use, copy, modify, and distribute this software and its
 *  documentation for any purpose and without fee is hereby granted, provided
 *  that the above copyright notice appear in all copies and supporting
 *  documentation and that the authors and the University of California
 *  are properly credited.  The authors and the University of California
 *  make no representations as to the suitability of this software for
 *  any purpose.  It is provided `as is', without express or implied warranty.
 */



/*
 *  IMPORTS
 *
 *  >>> Import descriptions:
 *  spConfig.h
 *     Macros that customize the sparse matrix routines.
 *  spmatrix.h
 *     Macros and declarations to be imported by the user.
 *  spDefs.h
 *     Matrix type and macro definitions for the sparse matrix routines.
 *  spFortran.h
 *     Definition of profiles
 */

#define spINSIDE_SPARSE
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"
#include "spFortran.h"

#ifdef FORTRAN






/*
 *  Example of a FORTRAN Program Calling Sparse
 *

      int matrix, error, sfCreate, sfGetElement, spFactor
      int element(10)
      double precision rhs(4), solution(4)
c
      matrix = sfCreate(4,0,error)
      element(1) = sfGetElement(matrix,1,1)
      element(2) = sfGetElement(matrix,1,2)
      element(3) = sfGetElement(matrix,2,1)
      element(4) = sfGetElement(matrix,2,2)
      element(5) = sfGetElement(matrix,2,3)
      element(6) = sfGetElement(matrix,3,2)
      element(7) = sfGetElement(matrix,3,3)
      element(8) = sfGetElement(matrix,3,4)
      element(9) = sfGetElement(matrix,4,3)
      element(10) = sfGetElement(matrix,4,4)
      call sfClear(matrix)
      call sfAdd1Real(element(1), 2d0)
      call sfAdd1Real(element(2), -1d0)
      call sfAdd1Real(element(3), -1d0)
      call sfAdd1Real(element(4), 3d0)
      call sfAdd1Real(element(5), -1d0)
      call sfAdd1Real(element(6), -1d0)
      call sfAdd1Real(element(7), 3d0)
      call sfAdd1Real(element(8), -1d0)
      call sfAdd1Real(element(9), -1d0)
      call sfAdd1Real(element(10), 3d0)
      call sfprint(matrix, .false., .false.)
      rhs(1) = 34d0
      rhs(2) = 0d0
      rhs(3) = 0d0
      rhs(4) = 0d0
      error = sfFactor(matrix)
      call sfSolve(matrix, rhs, solution)
      write (6, 10) rhs(1), rhs(2), rhs(3), rhs(4)
   10 format (f 10.2)
      end

 *
 */






long sfCreate(int  *Size, int  *Complex, int  *Error )
{
    /* Begin `sfCreate'. */
    return (long)spCreate(*Size, *Complex, Error );
}


void sfDestroy( long *Matrix )
{
    /* Begin `sfDestroy'. */
    spDestroy((char *)*Matrix);
    return;
}






#ifdef STRIP
void sfStripFills( long *Matrix )
{
    /* Begin `sfStripFills'. */
    spStripFills((char *)*Matrix);
    return;
}
#endif







/*
 *  CLEAR MATRIX
 *
 *  Sets every element of the matrix to zero and clears the error flag.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to matrix that is to be cleared.
 */

void sfClear( long *Matrix )
{
    /* Begin `sfClear'. */
    spClear((char *)*Matrix);
    return;
}






/*
 *  SINGLE ELEMENT ADDITION TO MATRIX BY INDEX
 *
 *  Finds element [Row,Col] and returns a pointer to it.  If element is
 *  not found then it is created and spliced into matrix.  This routine
 *  is only to be used after spCreate() and before spMNA_Preorder(),
 *  spFactor() or spOrderAndFactor().  Returns a pointer to the
 *  Real portion of a MatrixElement.  This pointer is later used by
 *  sfAddxxxxx() to directly access element.
 *
 *  >>> Returns: [INTEGER]
 *  Returns a pointer to the element.  This pointer is then used to directly
 *  access the element during successive builds.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to the matrix that the element is to be added to.
 *  Row  <input>  (int *) [INTEGER or INTEGER*2]
 *     Row index for element.  Must be in the range of [0..Size] unless
 *     the options EXPANDABLE or TRANSLATE are used. Elements placed in
 *     row zero are discarded.  In no case may Row be less than zero.
 *  Col  <input>  (int *) [INTEGER or INTEGER*2]
 *     Column index for element.  Must be in the range of [0..Size] unless
 *     the options EXPANDABLE or TRANSLATE are used. Elements placed in
 *     column zero are discarded.  In no case may Col be less than zero.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 *  Error is not cleared in this routine.
 */

long
sfGetElement( long *Matrix, int *Row, int *Col )
{
    /* Begin `sfGetElement'. */
    return (long)spGetElement((char *) * Matrix, *Row, *Col);
}







#ifdef QUAD_ELEMENT
/*
 *  ADDITION OF ADMITTANCE TO MATRIX BY INDEX
 *
 *  Performs same function as sfGetElement except rather than one
 *  element, all four Matrix elements for a floating component are
 *  added.  This routine also works if component is grounded.  Positive
 *  elements are placed at [Node1,Node2] and [Node2,Node1].  This
 *  routine is only to be used after sfCreate() and before
 *  sfMNA_Preorder(), sfFactor() or sfOrderAndFactor().
 *
 *  >>> Returns: [INTEGER or INTEGER*2]
 *  Error code.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to the matrix that component is to be entered in.
 *  Node1  <input>  (int *) [INTEGER or INTEGER*2]
 *     Row and column indices for elements. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Node zero is the
 *     ground node.  In no case may Node1 be less than zero.
 *  Node2  <input>  (int *) [INTEGER or INTEGER*2]
 *     Row and column indices for elements. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Node zero is the
 *     ground node.  In no case may Node2 be less than zero.
 *  Template  <output>  (long[4]) [INTEGER (4)]
 *     Collection of pointers to four elements that are later used to directly
 *     address elements.  User must supply the template, this routine will
 *     fill it.
 *
 *  Possible errors:
 *  spNO_MEMORY
 *  Error is not cleared in this routine.
 */

int
sfGetAdmittance( long *Matrix, int *Node1, int *Node2, long Template[4] )
{
    /* Begin `spGetAdmittance'. */
    return
        (   spGetAdmittance((char *) * Matrix, *Node1, *Node2,
                            (struct spTemplate *)Template )
        );
}
#endif /* QUAD_ELEMENT */









#ifdef QUAD_ELEMENT
/*
 *  ADDITION OF FOUR ELEMENTS TO MATRIX BY INDEX
 *
 *  Similar to sfGetAdmittance, except that sfGetAdmittance only
 *  handles 2-terminal components, whereas sfGetQuad handles simple
 *  4-terminals as well.  These 4-terminals are simply generalized
 *  2-terminals with the option of having the sense terminals different
 *  from the source and sink terminals.  sfGetQuad adds four
 *  elements to the matrix.  Positive elements occur at Row1,Col1
 *  Row2,Col2 while negative elements occur at Row1,Col2 and Row2,Col1.
 *  The routine works fine if any of the rows and columns are zero.
 *  This routine is only to be used after sfCreate() and before
 *  sfMNA_Preorder(), sfFactor() or sfOrderAndFactor()
 *  unless TRANSLATE is set true.
 *
 *  >>> Returns: [INTEGER or INTEGER*2]
 *  Error code.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to the matrix that component is to be entered in.
 *  Row1  <input>  (int *) [INTEGER or INTEGER*2]
 *     First row index for elements. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground row.  In no case may Row1 be less than zero.
 *  Row2  <input>  (int *) [INTEGER or INTEGER*2]
 *     Second row index for elements. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground row.  In no case may Row2 be less than zero.
 *  Col1  <input>  (int *) [INTEGER or INTEGER*2]
 *     First column index for elements. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground column.  In no case may Col1 be less than zero.
 *  Col2  <input>  (int *) [INTEGER or INTEGER*2]
 *     Second column index for elements. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground column.  In no case may Col2 be less than zero.
 *  Template  <output>  (long[4]) [INTEGER (4)]
 *     Collection of pointers to four elements that are later used to directly
 *     address elements.  User must supply the template, this routine will
 *     fill it.
 *
 *  Possible errors:
 *  spNO_MEMORY
 *  Error is not cleared in this routine.
 */

int
sfGetQuad( long  *Matrix, int  *Row1, int  *Row2, int  *Col1, int  *Col2, long Template[4] )
{
    /* Begin `spGetQuad'. */
    return
        (   spGetQuad( (char *) * Matrix, *Row1, *Row2, *Col1, *Col2,
                       (struct spTemplate *)Template )
        );
}
#endif /* QUAD_ELEMENT */









#ifdef QUAD_ELEMENT
/*
 *  ADDITION OF FOUR STRUCTURAL ONES TO MATRIX BY INDEX
 *
 *  Performs similar function to sfGetQuad() except this routine is
 *  meant for components that do not have an admittance representation.
 *
 *  The following stamp is used:
 *         Pos  Neg  Eqn
 *  Pos  [  .    .    1  ]
 *  Neg  [  .    .   -1  ]
 *  Eqn  [  1   -1    .  ]
 *
 *  >>> Returns: [INTEGER or INTEGER*2]
 *  Error code.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to the matrix that component is to be entered in.
 *  Pos  <input>  (int *) [INTEGER or INTEGER*2]
 *     See stamp above. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground row.  In no case may Pos be less than zero.
 *  Neg  <input>  (int *) [INTEGER or INTEGER*2]
 *     See stamp above. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground row.  In no case may Neg be less than zero.
 *  Eqn  <input>  (int *) [INTEGER or INTEGER*2]
 *     See stamp above. Must be in the range of [0..Size]
 *     unless the options EXPANDABLE or TRANSLATE are used. Zero is the
 *     ground row.  In no case may Eqn be less than zero.
 *  Template  <output>  (long[4]) [INTEGER (4)]
 *     Collection of pointers to four elements that are later used to directly
 *     address elements.  User must supply the template, this routine will
 *     fill it.
 *
 *  Possible errors:
 *  spNO_MEMORY
 *  Error is not cleared in this routine.
 */

int
sfGetOnes(long *Matrix, int *Pos, int *Neg, int *Eqn, long Template[4])
{
    /* Begin `sfGetOnes'. */
    return
        (   spGetOnes( (char *) * Matrix, *Pos, *Neg, *Eqn,
                       (struct spTemplate *)Template )
        );
}
#endif /* QUAD_ELEMENT */







/*
 *  ADD ELEMENT(S) DIRECTLY TO MATRIX
 *
 *  Adds a value to an element or a set of four element in a matrix.
 *  These elements are referenced by pointer, and so must already have
 *  been created by spGetElement(), spGetAdmittance(), spGetQuad(), or
 *  spGetOnes().
 *
 *  >>> Arguments:
 *  Element  <input>  (long *) [INTEGER]
 *      Pointer to the element that is to be added to.
 *  Template  <input>  (long[4]) [INTEGER (4)]
 *      Pointer to the element that is to be added to.
 *  Real  <input>  (spREAL *) [REAL or DOUBLE PRECISION]
 *      Real portion of the number to be added to the element.
 *  Imag  <input>  (spREAL *) [REAL or DOUBLE PRECISION]
 *      Imaginary portion of the number to be added to the element.
 */

void
sfAdd1Real( long *Element, RealNumber *Real )
{
    /* Begin `sfAdd1Real'. */
    *((RealNumber *)*Element) += *Real;
}


#ifdef spCOMPLEX

void
sfAdd1Imag( long *Element, RealNumber *Imag )
{
    /* Begin `sfAdd1Imag'. */
    *(((RealNumber *)*Element) + 1) += *Imag;
}


void
sfAdd1Complex( long *Element, RealNumber *Real, RealNumber *Imag )
{
    /* Begin `sfAdd1Complex'. */
    *((RealNumber *)*Element) += *Real;
    *(((RealNumber *)*Element) + 1) += *Imag;
}
#endif /* spCOMPLEX */


#ifdef QUAD_ELEMENT

void
sfAdd4Real( long Template[4], RealNumber *Real )
{
    /* Begin `sfAdd4Real'. */
    *((RealNumber *)Template[0]) += *Real;
    *((RealNumber *)Template[1]) += *Real;
    *((RealNumber *)Template[2]) -= *Real;
    *((RealNumber *)Template[3]) -= *Real;
}


#ifdef spCOMPLEX

void
sfAdd4Imag( long Template[4], RealNumber *Imag )
{
    /* Begin `sfAdd4Imag'. */
    *(((RealNumber *)Template[0]) + 1) += *Imag;
    *(((RealNumber *)Template[1]) + 1) += *Imag;
    *(((RealNumber *)Template[2]) + 1) -= *Imag;
    *(((RealNumber *)Template[3]) + 1) -= *Imag;
}


void
sfAdd4Complex( long Template[4], RealNumber *Real, RealNumber *Imag )
{
    /* Begin `sfAdd4Complex'. */
    *((RealNumber *)Template[0]) += *Real;
    *((RealNumber *)Template[1]) += *Real;
    *((RealNumber *)Template[2]) -= *Real;
    *((RealNumber *)Template[3]) -= *Real;
    *(((RealNumber *)Template[0]) + 1) += *Imag;
    *(((RealNumber *)Template[1]) + 1) += *Imag;
    *(((RealNumber *)Template[2]) + 1) -= *Imag;
    *(((RealNumber *)Template[3]) + 1) -= *Imag;
}
#endif /* spCOMPLEX */
#endif /* QUAD_ELEMENT */






/*
 *  ORDER AND FACTOR MATRIX
 *
 *  This routine chooses a pivot order for the matrix and factors it
 *  into LU form.  It handles both the initial factorization and subsequent
 *  factorizations when a reordering is desired.  This is handled in a manner
 *  that is transparent to the user.  The routine uses a variation of
 *  Gauss's method where the pivots are associated with L and the
 *  diagonal terms of U are one.
 *
 *  >>> Returned: [INTEGER of INTEGER*2]
 *  The error code is returned.  Possible errors are listed below.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *  RHS  <input>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      Representative right-hand side vector that is used to determine
 *      pivoting order when the right hand side vector is sparse.  If
 *      RHS is a NULL pointer then the RHS vector is assumed to
 *      be full and it is not used when determining the pivoting
 *      order.
 *  RelThreshold  <input>  (RealNumber *) [REAL or DOUBLE PRECISION]
 *      This number determines what the pivot relative threshold will
 *      be.  It should be between zero and one.  If it is one then the
 *      pivoting method becomes complete pivoting, which is very slow
 *      and tends to fill up the matrix.  If it is set close to zero
 *      the pivoting method becomes strict Markowitz with no
 *      threshold.  The pivot threshold is used to eliminate pivot
 *      candidates that would cause excessive element growth if they
 *      were used.  Element growth is the cause of roundoff error.
 *      Element growth occurs even in well-conditioned matrices.
 *      Setting the RelThreshold large will reduce element growth and
 *      roundoff error, but setting it too large will cause execution
 *      time to be excessive and will result in a large number of
 *      fill-ins.  If this occurs, accuracy can actually be degraded
 *      because of the large number of operations required on the
 *      matrix due to the large number of fill-ins.  A good value seems
 *      to be 0.001.  The default is chosen by giving a value larger
 *      than one or less than or equal to zero.  This value should be
 *      increased and the matrix resolved if growth is found to be
 *      excessive.  Changing the pivot threshold does not improve
 *      performance on matrices where growth is low, as is often the
 *      case with ill-conditioned matrices.  Once a valid threshold is
 *      given, it becomes the new default.  The default value of
 *      RelThreshold was chosen for use with nearly diagonally
 *      dominant matrices such as node- and modified-node admittance
 *      matrices.  For these matrices it is usually best to use
 *      diagonal pivoting.  For matrices without a strong diagonal, it
 *      is usually best to use a larger threshold, such as 0.01 or
 *      0.1.
 *  AbsThreshold  <input>  (RealNumber *) [REAL or DOUBLE PRECISION]
 *      The absolute magnitude an element must have to be considered
 *      as a pivot candidate, except as a last resort.  This number
 *      should be set significantly smaller than the smallest diagonal
 *      element that is is expected to be placed in the matrix.  If
 *      there is no reasonable prediction for the lower bound on these
 *      elements, then AbsThreshold should be set to zero.
 *      AbsThreshold is used to reduce the possibility of choosing as a
 *      pivot an element that has suffered heavy cancellation and as a
 *      result mainly consists of roundoff error.  Once a valid
 *      threshold is given, it becomes the new default.
 *  DiagPivoting  <input>  (long *) [LOGICAL]
 *      A flag indicating that pivot selection should be confined to the
 *      diagonal if possible.  If DiagPivoting is nonzero and if
 *      DIAGONAL_PIVOTING is enabled pivots will be chosen only from
 *      the diagonal unless there are no diagonal elements that satisfy
 *      the threshold criteria.  Otherwise, the entire reduced
 *      submatrix is searched when looking for a pivot.  The diagonal
 *      pivoting in Sparse is efficient and well refined, while the
 *      off-diagonal pivoting is not.  For symmetric and near symmetric
 *      matrices, it is best to use diagonal pivoting because it
 *      results in the best performance when reordering the matrix and
 *      when factoring the matrix without ordering.  If there is a
 *      considerable amount of nonsymmetry in the matrix, then
 *      off-diagonal pivoting may result in a better equation ordering
 *      simply because there are more pivot candidates to choose from.
 *      A better ordering results in faster subsequent factorizations.
 *      However, the initial pivot selection process takes considerably
 *      longer for off-diagonal pivoting.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 *  spSINGULAR
 *  spSMALL_PIVOT
 *  Error is cleared in this function.
 */

int
sfOrderAndFactor( long *Matrix, RealNumber RHS[], RealNumber *RelThreshold, RealNumber* AbsThreshold, long *DiagPivoting )
{
    /* Begin `sfOrderAndFactor'. */
    return spOrderAndFactor( (char *) * Matrix, RHS, *RelThreshold,
                             *AbsThreshold, (SPBOOLEAN) * DiagPivoting );
}







/*
 *  FACTOR MATRIX
 *
 *  This routine is the companion routine to spOrderAndFactor().
 *  Unlike sfOrderAndFactor(), sfFactor() cannot change the ordering.
 *  It is also faster than sfOrderAndFactor().  The standard way of
 *  using these two routines is to first use sfOrderAndFactor() for the
 *  initial factorization.  For subsequent factorizations, sfFactor()
 *  is used if there is some assurance that little growth will occur
 *  (say for example, that the matrix is diagonally dominant).  If
 *  sfFactor() is called for the initial factorization of the matrix,
 *  then sfOrderAndFactor() is automatically called with the default
 *  threshold.  This routine uses "row at a time" LU factorization.
 *  Pivots are associated with the lower triangular matrix and the
 *  diagonals of the upper triangular matrix are ones.
 *
 *  >>> Returned: [INTEGER or INTEGER*2]
 *  The error code is returned.  Possible errors are listed below.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 *  spSINGULAR
 *  spZERO_DIAG
 *  spSMALL_PIVOT
 *  Error is cleared in this function.
 */

int
sfFactor( long *Matrix )
{
    /* Begin `sfFactor'. */
    return spFactor((char *) * Matrix);
}






/*
 *  PARTITION MATRIX
 *
 *  This routine determines the cost to factor each row using both
 *  direct and indirect addressing and decides, on a row-by-row basis,
 *  which addressing mode is fastest.  This information is used in
 *  sfFactor() to speed the factorization.
 *
 *  When factoring a previously ordered matrix using sfFactor(), Sparse
 *  operates on a row-at-a-time basis.  For speed, on each step, the
 *  row being updated is copied into a full vector and the operations
 *  are performed on that vector.  This can be done one of two ways,
 *  either using direct addressing or indirect addressing.  Direct
 *  addressing is fastest when the matrix is relatively dense and
 *  indirect addressing is best when the matrix is quite sparse.  The
 *  user selects the type of partition used with Mode.  If Mode is set
 *  to spDIRECT_PARTITION, then the all rows are placed in the direct
 *  addressing partition.  Similarly, if Mode is set to
 *  spINDIRECT_PARTITION, then the all rows are placed in the indirect
 *  addressing partition.  By setting Mode to spAUTO_PARTITION, the
 *  user allows Sparse to select the partition for each row
 *  individually.  sfFactor() generally runs faster if Sparse is
 *  allowed to choose its own partitioning, however choosing a
 *  partition is expensive.  The time required to choose a partition is
 *  of the same order of the cost to factor the matrix.  If you plan to
 *  factor a large number of matrices with the same structure, it is
 *  best to let Sparse choose the partition.  Otherwise, you should
 *  choose the partition based on the predicted density of the matrix.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *  Mode  <input>  (int *) [INTEGER or INTEGER*2]
 *      Mode must be one of three special codes: spDIRECT_PARTITION,
 *      spINDIRECT_PARTITION, or spAUTO_PARTITION.
 */

void
sfPartition( long *Matrix, int *Mode )
{
    /* Begin `sfPartition'. */
    spPartition((char *)*Matrix, *Mode);
}







/*
 *  SOLVE MATRIX EQUATION
 *
 *  Performs forward elimination and back substitution to find the
 *  unknown vector from the RHS vector and factored matrix.  This
 *  routine assumes that the pivots are associated with the lower
 *  triangular (L) matrix and that the diagonal of the upper triangular
 *  (U) matrix consists of ones.  This routine arranges the computation
 *  in different way than is traditionally used in order to exploit the
 *  sparsity of the right-hand side.  See the reference in spRevision.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *  RHS  <input>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      RHS is the input data array, the right hand side. This data is
 *      undisturbed and may be reused for other solves.
 *  Solution  <output>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      Solution is the output data array. This routine is constructed such that
 *      RHS and Solution can be the same array.
 *  iRHS  <input>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      iRHS is the imaginary portion of the input data array, the right
 *      hand side. This data is undisturbed and may be reused for other solves.
 *      This argument is only necessary if matrix is complex and if
 *      spSEPARATED_COMPLEX_VECTOR is set true.
 *  iSolution  <output>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      iSolution is the imaginary portion of the output data array. This
 *      routine is constructed such that iRHS and iSolution can be
 *      the same array.  This argument is only necessary if matrix is complex
 *      and if spSEPARATED_COMPLEX_VECTOR is set true.
 *
 *  >>> Obscure Macros
 *  IMAG_VECTORS
 *      Replaces itself with `, iRHS, iSolution' if the options spCOMPLEX and
 *      spSEPARATED_COMPLEX_VECTORS are set, otherwise it disappears
 *      without a trace.
 */

/*VARARGS3*/

void
sfSolve( long *Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS )
{
    /* Begin `sfSolve'. */
    spSolve( (char *)*Matrix, RHS, Solution IMAG_VECTORS );
}






#ifdef TRANSPOSE
/*
 *  SOLVE TRANSPOSED MATRIX EQUATION
 *
 *  Performs forward elimination and back substitution to find the
 *  unknown vector from the RHS vector and transposed factored
 *  matrix. This routine is useful when performing sensitivity analysis
 *  on a circuit using the adjoint method.  This routine assumes that
 *  the pivots are associated with the untransposed lower triangular
 *  (L) matrix and that the diagonal of the untransposed upper
 *  triangular (U) matrix consists of ones.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *  RHS  <input>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      RHS is the input data array, the right hand side. This data is
 *      undisturbed and may be reused for other solves.
 *  Solution  <output>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      Solution is the output data array. This routine is constructed such that
 *      RHS and Solution can be the same array.
 *  iRHS  <input>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      iRHS is the imaginary portion of the input data array, the right
 *      hand side. This data is undisturbed and may be reused for other solves.
 *      If spSEPARATED_COMPLEX_VECTOR is set false, or if matrix is real, there
 *      is no need to supply this array.
 *  iSolution  <output>  (RealVector) [REAL (1) or DOUBLE PRECISION (1)]
 *      iSolution is the imaginary portion of the output data array. This
 *      routine is constructed such that iRHS and iSolution can be
 *      the same array.  If spSEPARATED_COMPLEX_VECTOR is set false, or if
 *      matrix is real, there is no need to supply this array.
 *
 *  >>> Obscure Macros
 *  IMAG_VECTORS
 *      Replaces itself with `, iRHS, iSolution' if the options spCOMPLEX and
 *      spSEPARATED_COMPLEX_VECTORS are set, otherwise it disappears
 *      without a trace.
 */

/*VARARGS3*/

void
sfSolveTransposed( long *Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS )
{
    /* Begin `sfSolveTransposed'. */
    spSolveTransposed( (char *)*Matrix, RHS, Solution IMAG_VECTORS );
}
#endif /* TRANSPOSE */





#ifdef DOCUMENTATION
/*
 *  PRINT MATRIX
 *
 *  Formats and send the matrix to standard output.  Some elementary
 *  statistics are also output.  The matrix is output in a format that is
 *  readable by people.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *  PrintReordered  <input>  (long *) [LOGICAL]
 *      Indicates whether the matrix should be printed out in its original
 *      form, as input by the user, or whether it should be printed in its
 *      reordered form, as used by the matrix routines.  Zero indicates that
 *      the matrix should be printed as inputed, one indicates that it
 *      should be printed reordered.
 *  Data  <input>  (long *) [LOGICAL]
 *      Boolean flag that when false indicates that output should be
 *      compressed such that only the existence of an element should be
 *      indicated rather than giving the actual value. Thus 10 times as many
 *      can be printed on a row.  A zero signifies that the matrix should
 *      be printed compressed.  A one indicates that the matrix should be
 *      printed in all its glory.
 *  Header  <input>  (long *) [LOGICAL]
 *      Flag indicating that extra information such as the row and column
 *      numbers should be printed.
 */

void
sfPrint( long *Matrix, long *Data, long *PrintReordered, long *Header )
{
    /* Begin `sfPrint'. */
    spPrint( (char *)*Matrix, (int)*PrintReordered, (int)*Data, (int)*Header );
}
#endif /* DOCUMENTATION */






#ifdef DOCUMENTATION
/*
 *  OUTPUT MATRIX TO FILE
 *
 *  Writes matrix to file in format suitable to be read back in by the
 *  matrix test program.  Data is sent to a file with a fixed name because
 *  it is impossible to pass strings from FORTRAN to C in a manner that is
 *  portable.
 *
 *  >>> Returns:
 *  One is returned if routine was successful, otherwise zero is returned.
 *  The calling function can query errno (the system global error variable)
 *  as to the reason why this routine failed.
 *
 *  >>> Arguments: [LOGICAL]
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 *  Reordered  <input> (long *) [LOGICAL]
 *      Specifies whether matrix should be output in reordered form,
 *      or in original order.
 *  Data  <input> (long *) [LOGICAL]
 *      Indicates that the element values should be output along with
 *      the indices for each element.  This parameter must be true if
 *      matrix is to be read by the sparse test program.
 *  Header  <input> (long *) [LOGICAL]
 *      Indicates that header is desired.  This parameter must be true if
 *      matrix is to be read by the sparse test program.
 */
#define MATRIX_FILE_NAME        "spMatrix"
#define STATS_FILE_NAME         "spStats"

long
sfFileMatrix( long *Matrix, long *Reordered, long *Data, long *Header )
{
    /* Begin `sfFileMatrix'. */
    return spFileMatrix( (char *) * Matrix, MATRIX_FILE_NAME, "",
                         (int) * Reordered, (int) * Data, (int) * Header );
}
#endif /* DOCUMENTATION */



#ifdef DOCUMENTATION
/*
 *  OUTPUT SOURCE VECTOR TO FILE
 *
 *  Writes vector to file in format suitable to be read back in by the
 *  matrix test program.  This routine should be executed after the function
 *  sfFileMatrix.
 *
 *  >>> Returns:
 *  One is returned if routine was successful, otherwise zero is returned.
 *  The calling function can query errno (the system global error variable)
 *  as to the reason why this routine failed.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *)
 *      Pointer to matrix.
 *  RHS  <input>  (RealNumber []) [REAL (1) or DOUBLE PRECISION (1)]
 *      Right-hand side vector. This is only the real portion if
 *      spSEPARATED_COMPLEX_VECTORS is true.
 *  iRHS  <input>  (RealNumber []) [REAL (1) or DOUBLE PRECISION (1)]
 *      Right-hand side vector, imaginary portion.  Not necessary if matrix
 *      is real or if spSEPARATED_COMPLEX_VECTORS is set false.
 */

int
sfFileVector( long *Matrix, RealVector RHS IMAG_RHS )
{
    /* Begin `sfFileVector'. */
    return spFileVector( (char *) * Matrix, MATRIX_FILE_NAME, RHS IMAG_RHS );
}
#endif /* DOCUMENTATION */







#ifdef DOCUMENTATION
/*
 *  OUTPUT STATISTICS TO FILE
 *
 *  Writes useful information concerning the matrix to a file.  Should be
 *  executed after the matrix is factored.
 *
 *  >>> Returns: [LOGICAL]
 *  One is returned if routine was successful, otherwise zero is returned.
 *  The calling function can query errno (the system global error variable)
 *  as to the reason why this routine failed.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to matrix.
 */

int
sfFileStats( long *Matrix )
{
    /* Begin `sfFileStats'. */
    return spFileStats( (char *) * Matrix, STATS_FILE_NAME, "" );
}
#endif /* DOCUMENTATION */




#ifdef MODIFIED_NODAL
/*
 *  PREORDER MODIFIED NODE ADMITTANCE MATRIX TO REMOVE ZEROS FROM DIAGONAL
 *
 *  This routine massages modified node admittance matrices to remove
 *  zeros from the diagonal.  It takes advantage of the fact that the
 *  row and column associated with a zero diagonal usually have
 *  structural ones placed symmetricly.  This routine should be used
 *  only on modified node admittance matrices and should be executed
 *  after the matrix has been built but before the factorization
 *  begins.  It should be executed for the initial factorization only
 *  and should be executed before the rows have been linked.  Thus it
 *  should be run before using spScale(), spMultiply(),
 *  spDeleteRowAndCol(), or spNorm().
 *
 *  This routine exploits the fact that the structural one are placed
 *  in the matrix in symmetric twins.  For example, the stamps for
 *  grounded and a floating voltage sources are
 *  grounded:              floating:
 *  [  x   x   1 ]         [  x   x   1 ]
 *  [  x   x     ]         [  x   x  -1 ]
 *  [  1         ]         [  1  -1     ]
 *  Notice for the grounded source, there is one set of twins, and for
 *  the grounded, there are two sets.  We remove the zero from the diagonal
 *  by swapping the rows associated with a set of twins.  For example:
 *  grounded:              floating 1:            floating 2:
 *  [  1         ]         [  1  -1     ]         [  x   x   1 ]
 *  [  x   x     ]         [  x   x  -1 ]         [  1  -1     ]
 *  [  x   x   1 ]         [  x   x   1 ]         [  x   x  -1 ]
 *
 *  It is important to deal with any zero diagonals that only have one
 *  set of twins before dealing with those that have more than one because
 *  swapping row destroys the symmetry of any twins in the rows being
 *  swapped, which may limit future moves.  Consider
 *  [  x   x   1     ]
 *  [  x   x  -1   1 ]
 *  [  1  -1         ]
 *  [      1         ]
 *  There is one set of twins for diagonal 4 and two for diagonal3.
 *  Dealing with diagonal for first requires swapping rows 2 and 4.
 *  [  x   x   1     ]
 *  [      1         ]
 *  [  1  -1         ]
 *  [  x   x  -1   1 ]
 *  We can now deal with diagonal 3 by swapping rows 1 and 3.
 *  [  1  -1         ]
 *  [      1         ]
 *  [  x   x   1     ]
 *  [  x   x  -1   1 ]
 *  And we are done, there are no zeros left on the diagonal.  However, if
 *  we originally dealt with diagonal 3 first, we could swap rows 2 and 3
 *  [  x   x   1     ]
 *  [  1  -1         ]
 *  [  x   x  -1   1 ]
 *  [      1         ]
 *  Diagonal 4 no longer has a symmetric twin and we cannot continue.
 *
 *  So we always take care of lone twins first.  When none remain, we
 *  choose arbitrarily a set of twins for a diagonal with more than one set
 *  and swap the rows corresponding to that twin.  We then deal with any
 *  lone twins that were created and repeat the procedure until no
 *  zero diagonals with symmetric twins remain.
 *
 *  In this particular implementation, columns are swapped rather than rows.
 *  The algorithm used in this function was developed by Ken Kundert and
 *  Tom Quarles.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to the matrix to be preordered.
 */

void
sfMNA_Preorder( long *Matrix )
{
    /* Begin `sfMNA_Preorder'. */
    spMNA_Preorder( (char *)*Matrix );
}
#endif /* MODIFIED_NODAL */






#ifdef SCALING
/*
 *  SCALE MATRIX
 *
 *  This function scales the matrix to enhance the possibility of
 *  finding a good pivoting order.  Note that scaling enhances accuracy
 *  of the solution only if it affects the pivoting order, so it makes
 *  no sense to scale the matrix before spFactor().  If scaling is
 *  desired it should be done before spOrderAndFactor().  There
 *  are several things to take into account when choosing the scale
 *  factors.  First, the scale factors are directly multiplied against
 *  the elements in the matrix.  To prevent roundoff, each scale factor
 *  should be equal to an int power of the number base of the
 *  machine.  Since most machines operate in base two, scale factors
 *  should be a power of two.  Second, the matrix should be scaled such
 *  that the matrix of element uncertainties is equilibrated.  Third,
 *  this function multiplies the scale factors by the elements, so if
 *  one row tends to have uncertainties 1000 times smaller than the
 *  other rows, then its scale factor should be 1024, not 1/1024.
 *  Fourth, to save time, this function does not scale rows or columns
 *  if their scale factors are equal to one.  Thus, the scale factors
 *  should be normalized to the most common scale factor.  Rows and
 *  columns should be normalized separately.  For example, if the size
 *  of the matrix is 100 and 10 rows tend to have uncertainties near
 *  1e-6 and the remaining 90 have uncertainties near 1e-12, then the
 *  scale factor for the 10 should be 1/1,048,576 and the scale factors
 *  for the remaining 90 should be 1.  Fifth, since this routine
 *  directly operates on the matrix, it is necessary to apply the scale
 *  factors to the RHS and Solution vectors.  It may be easier to
 *  simply use spOrderAndFactor() on a scaled matrix to choose the
 *  pivoting order, and then throw away the matrix.  Subsequent
 *  factorizations, performed with spFactor(), will not need to have
 *  the RHS and Solution vectors descaled.  Lastly, this function
 *  should not be executed before the function spMNA_Preorder.
 *
 *  >>> Arguments:
 *  Matrix  <input> (long *) [INTEGER]
 *      Pointer to the matrix to be scaled.
 *  SolutionScaleFactors  <input>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      The array of Solution scale factors.  These factors scale the columns.
 *      All scale factors are real valued.
 *  RHS_ScaleFactors  <input>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      The array of RHS scale factors.  These factors scale the rows.
 *      All scale factors are real valued.
 */

void
sfScale( long *Matrix, RealVector RHS_ScaleFactors, RealVector SolutionScaleFactors )
{
    /* Begin `sfScale'. */
    spScale( (char *)*Matrix, RHS_ScaleFactors, SolutionScaleFactors );
}
#endif /* SCALING */






#ifdef MULTIPLICATION
/*
 *  MATRIX MULTIPLICATION
 *
 *  Multiplies matrix by solution vector to find source vector.
 *  Assumes matrix has not been factored.  This routine can be used
 *  as a test to see if solutions are correct.  It should not be used
 *  before PreorderFoModifiedNodal().
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to the matrix.
 *  RHS  <output>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      RHS is the right hand side. This is what is being solved for.
 *  Solution  <input>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      Solution is the vector being multiplied by the matrix.
 *  iRHS  <output>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      iRHS is the imaginary portion of the right hand side. This is
 *      what is being solved for.  This is only necessary if the matrix is
 *      complex and spSEPARATED_COMPLEX_VECTORS is true.
 *  iSolution  <input>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      iSolution is the imaginary portion of the vector being multiplied
 *      by the matrix. This is only necessary if the matrix is
 *      complex and spSEPARATED_COMPLEX_VECTORS is true.
 *
 *  >>> Obscure Macros
 *  IMAG_VECTORS
 *      Replaces itself with `, iRHS, iSolution' if the options spCOMPLEX and
 *      spSEPARATED_COMPLEX_VECTORS are set, otherwise it disappears
 *      without a trace.
 */

void
sfMultiply( long *Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS )
{
    /* Begin `sfMultiply'. */
    spMultiply( (char *)*Matrix, RHS, Solution IMAG_VECTORS );
}
#endif /* MULTIPLICATION */






#if MULTIPLICATION AND TRANSPOSE
/*
 *  TRANSPOSED MATRIX MULTIPLICATION
 *
 *  Multiplies transposed matrix by solution vector to find source vector.
 *  Assumes matrix has not been factored.  This routine can be used
 *  as a test to see if solutions are correct.  It should not be used
 *  before PreorderFoModifiedNodal().
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to the matrix.
 *  RHS  <output>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      RHS is the right hand side. This is what is being solved for.
 *  Solution  <input>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      Solution is the vector being multiplied by the matrix.
 *  iRHS  <output>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      iRHS is the imaginary portion of the right hand side. This is
 *      what is being solved for.  This is only necessary if the matrix is
 *      complex and spSEPARATED_COMPLEX_VECTORS is true.
 *  iSolution  <input>  (RealVector) [REAL(1) or DOUBLE PRECISION(1)]
 *      iSolution is the imaginary portion of the vector being multiplied
 *      by the matrix. This is only necessary if the matrix is
 *      complex and spSEPARATED_COMPLEX_VECTORS is true.
 *
 *  >>> Obscure Macros
 *  IMAG_VECTORS
 *      Replaces itself with `, iRHS, iSolution' if the options spCOMPLEX and
 *      spSEPARATED_COMPLEX_VECTORS are set, otherwise it disappears
 *      without a trace.
 */

void
sfMultTransposed( long *Matrix, RealVector RHS, RealVector Solution IMAG_VECTORS )
{
    /* Begin `sfMultTransposed'. */
    spMultTransposed( (char *)*Matrix, RHS, Solution IMAG_VECTORS );
}
#endif
/* MULTIPLICATION AND TRANSPOSE */



#ifdef DETERMINANT

/*
 *  CALCULATE DETERMINANT
 *
 *  This routine in capable of calculating the determinant of the
 *  matrix once the LU factorization has been performed.  Hence, only
 *  use this routine after spFactor() and before spClear().
 *  The determinant equals the product of all the diagonal elements of
 *  the lower triangular matrix L, except that this product may need
 *  negating.  Whether the product or the negative product equals the
 *  determinant is determined by the number of row and column
 *  interchanges performed.  Note that the determinants of matrices can
 *  be very large or very small.  On large matrices, the determinant
 *  can be far larger or smaller than can be represented by a floating
 *  point number.  For this reason the determinant is scaled to a
 *  reasonable value and the logarithm of the scale factor is returned.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      A pointer to the matrix for which the determinant is desired.
 *  pExponent  <output>  (int *) [INTEGER or INTEGER*2]
 *      The logarithm base 10 of the scale factor for the determinant.  To
 *      find
 *      the actual determinant, Exponent should be added to the exponent of
 *      DeterminantReal.
 *  pDeterminant  <output>  (RealNumber *)  [REAL or DOUBLE PRECISION]
 *      The real portion of the determinant.   This number is scaled to be
 *      greater than or equal to 1.0 and less than 10.0.
 *  piDeterminant  <output>  (RealNumber *) [REAL or DOUBLE PRECISION]
 *      The imaginary portion of the determinant.  When the matrix is real
 *      this pointer need not be supplied, nothing will be returned.   This
 *      number is scaled to be greater than or equal to 1.0 and less than 10.0.
 */

#ifdef spCOMPLEX

void
sfDeterminant( long *Matrix, int  *pExponent, RealNumber *pDeterminant, RealNumber *piDeterminant )
{
    /* Begin `sfDeterminant'. */
    spDeterminant( (char *)*Matrix, pExponent, pDeterminant, piDeterminant );
}

#else /* spCOMPLEX */

void
sfDeterminant( long *Matrix, int  *pExponent, RealNumber *pDeterminant )
{
    /* Begin `sfDeterminant'. */
    spDeterminant( (char *)*Matrix, pExponent, pDeterminant );
}
#endif /* spCOMPLEX */
#endif /* DETERMINANT */






/*
 *  RETURN MATRIX ERROR STATUS
 *
 *  This function is used to determine the error status of the given matrix.
 *
 *  >>> Returned: [INTEGER or INTEGER*2]
 *     The error status of the given matrix.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     The matrix for which the error status is desired.
 */

int
sfError( long *Matrix )
{
    /* Begin `sfError'. */
    return spError( (char *) * Matrix );
}






/*
 *  WHERE IS MATRIX SINGULAR
 *
 *  This function returns the row and column number where the matrix was
 *  detected as singular or where a zero was detected on the diagonal.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     The matrix for which the error status is desired.
 *  pRow  <output>  (int *) [INTEGER or INTEGER*2]
 *     The row number.
 *  pCol  <output>  (int *) [INTEGER or INTEGER*2]
 *     The column number.
 */

void
sfWhereSingular( long *Matrix, int *Row, int *Col )
{
    /* Begin `sfWhereSingular'. */
    spWhereSingular( (char *)*Matrix, Row, Col );
}





/*
 *   MATRIX SIZE
 *
 *   Returns the size of the matrix.  Either the internal or external size of
 *   the matrix is returned.
 *
 *   >>> Arguments:
 *   Matrix  <input>  (long *) [INTEGER]
 *       Pointer to matrix.
 *   External  <input>  (SPBOOLEAN) [LOGICAL]
 *       If External is set true, the external size , i.e., the value of the
 *       largest external row or column number encountered is returned.
 *       Otherwise the true size of the matrix is returned.  These two sizes
 *       may differ if the TRANSLATE option is set true.
 */

int
sfGetSize( long *Matrix, long *External )
{
    /* Begin `sfGetSize'. */
    return spGetSize( (char *) * Matrix, (SPBOOLEAN) * External );
}








/*
 *   SET MATRIX COMPLEX OR REAL
 *
 *   Forces matrix to be either real or complex.
 *
 *   >>> Arguments:
 *   Matrix  <input>  (long *) [INTEGER]
 *       Pointer to matrix.
 */

void
sfSetReal( long *Matrix )
{
    /* Begin `sfSetReal'. */
    spSetReal( (char *)*Matrix );
}


void
sfSetComplex( long *Matrix )
{
    /* Begin `sfSetComplex'. */
    spSetComplex( (char *)*Matrix );
}









/*
 *   ELEMENT OR FILL-IN COUNT
 *
 *   Two functions used to return simple statistics.  Either the number
 *   of total elements, or the number of fill-ins can be returned.
 *
 *   >>> Arguments:
 *   Matrix  <input>  (long *) [INTEGER]
 *       Pointer to matrix.
 */

int
sfFillinCount( long *Matrix )
{
    /* Begin `sfFillinCount'. */
    return spFillinCount( (char *) * Matrix );
}


int
sfElementCount( long *Matrix )
{
    /* Begin `sfElementCount'. */
    return spElementCount( (char *) * Matrix );
}






#if TRANSLATE AND DELETE

/*
 *  DELETE A ROW AND COLUMN FROM THE MATRIX
 *
 *  Deletes a row and a column from a matrix.
 *
 *  Sparse will abort if an attempt is made to delete a row or column that
 *  doesn't exist.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to the matrix in which the row and column are to be deleted.
 *  Row  <input>  (int) [INTEGER or INTEGER*2]
 *     Row to be deleted.
 *  Col  <input>  (int) [INTEGER or INTEGER*2]
 *     Column to be deleted.
 */

void
sfDeleteRowAndCol( long *Matrix, int *Row, int *Col )
{
    /* Begin `sfDeleteRowAndCol'. */
    spDeleteRowAndCol( (char *)*Matrix, *Row, *Col );
}
#endif





#ifdef PSEUDOCONDITION

/*
 *  CALCULATE PSEUDOCONDITION
 *
 *  Computes the magnitude of the ratio of the largest to the smallest
 *  pivots.  This quantity is an indicator of ill-conditioning in the
 *  matrix.  If this ratio is large, and if the matrix is scaled such
 *  that uncertainties in the RHS and the matrix entries are
 *  equilibrated, then the matrix is ill-conditioned.  However, a small
 *  ratio does not necessarily imply that the matrix is
 *  well-conditioned.  This routine must only be used after a matrix
 *  has been factored by sfOrderAndFactor() or sfFactor() and before it
 *  is cleared by sfClear() or spInitialize().  The pseudocondition is faster
 *  to compute than the condition number calculated by sfCondition(), but
 *  is not as informative.
 *
 *  >>> Returns: [REAL or DOUBLE PRECISION]
 *  The magnitude of the ratio of the largest to smallest pivot used during
 *  previous factorization.  If the matrix was singular, zero is returned.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *)
 *     Pointer to the matrix.
 */

RealNumber sfPseudoCondition( long *Matrix )
{
    /* Begin `sfPseudoCondition'. */
    return spPseudoCondition( (char *)Matrix );
}
#endif







#ifdef CONDITION

/*
 *  ESTIMATE CONDITION NUMBER
 *
 *  Computes an estimate of the condition number using a variation on
 *  the LINPACK condition number estimation algorithm.  This quantity is
 *  an indicator of ill-conditioning in the matrix.  To avoid problems
 *  with overflow, the reciprocal of the condition number is returned.
 *  If this number is small, and if the matrix is scaled such that
 *  uncertainties in the RHS and the matrix entries are equilibrated,
 *  then the matrix is ill-conditioned.  If the this number is near
 *  one, the matrix is well conditioned.  This routine must only be
 *  used after a matrix has been factored by sfOrderAndFactor() or
 *  sfFactor() and before it is cleared by sfClear() or spInitialize().
 *
 *  Unlike the LINPACK condition number estimator, this routines
 *  returns the L infinity condition number.  This is an artifact of
 *  Sparse placing ones on the diagonal of the upper triangular matrix
 *  rather than the lower.  This difference should be of no importance.
 *
 *  References:
 *  A.K. Cline, C.B. Moler, G.W. Stewart, J.H. Wilkinson.  An estimate
 *  for the condition number of a matrix.  SIAM Journal on Numerical
 *  Analysis.  Vol. 16, No. 2, pages 368-375, April 1979.
 *
 *  J.J. Dongarra, C.B. Moler, J.R. Bunch, G.W. Stewart.  LINPACK
 *  User's Guide.  SIAM, 1979.
 *
 *  Roger G. Grimes, John G. Lewis.  Condition number estimation for
 *  sparse matrices.  SIAM Journal on Scientific and Statistical
 *  Computing.  Vol. 2, No. 4, pages 384-388, December 1981.
 *
 *  Dianne Prost O'Leary.  Estimating matrix condition numbers.  SIAM
 *  Journal on Scientific and Statistical Computing.  Vol. 1, No. 2,
 *  pages 205-209, June 1980.
 *
 *  >>> Returns: [REAL or DOUBLE PRECISION]
 *  The reciprocal of the condition number.  If the matrix was singular,
 *  zero is returned.
 *
 *  >>> Arguments:
 *  eMatrix  <input>  (long *)
 *      Pointer to the matrix.
 *  NormOfMatrix  <input>  (RealNumber *) [REAL or DOUBLE PRECISION]
 *      The L-infinity norm of the unfactored matrix as computed by
 *      spNorm().
 *  pError  <output>  (int *) [INTEGER or INTEGER*2]
 *      Used to return error code.
 *
 *  >>> Possible errors:
 *  spSINGULAR
 *  spNO_MEMORY
 */

RealNumber sfCondition( long * Matrix, RealNumber *NormOfMatrix, int *pError )
{
    /* Begin `sfCondition'. */
    return spCondition( (char *) * Matrix, *NormOfMatrix, pError );
}



/*
 *  L-INFINITY MATRIX NORM
 *
 *  Computes the L-infinity norm of an unfactored matrix.  It is a fatal
 *  error to pass this routine a factored matrix.
 *
 *  One difficulty is that the rows may not be linked.
 *
 *  >>> Returns: [REAL or DOUBLE PRECISION]
 *  The largest absolute row sum of matrix.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *)
 *     Pointer to the matrix.
 */

RealNumber sfNorm( long *Matrix )
{
    /* Begin `sfNorm'. */
    return spNorm( (char *) * Matrix );
}
#endif /* CONDITION */





#ifdef STABILITY

/*
 *  STABILITY OF FACTORIZATION
 *
 *  The following routines are used to gauge the stability of a
 *  factorization.  If the factorization is determined to be too unstable,
 *  then the matrix should be reordered.  The routines compute quantities
 *  that are needed in the computation of a bound on the error attributed
 *  to any one element in the matrix during the factorization.  In other
 *  words, there is a matrix E = [e_ij] of error terms such that A+E = LU.
 *  This routine finds a bound on |e_ij|.  Erisman & Reid [1] showed that
 *  |e_ij| < 3.01 u rho m_ij, where u is the machine rounding unit,
 *  rho = max a_ij where the max is taken over every row i, column j, and
 *  step k, and m_ij is the number of multiplications required in the
 *  computation of l_ij if i > j or u_ij otherwise.  Barlow [2] showed that
 *  rho < max_i || l_i ||_p max_j || u_j ||_q where 1/p + 1/q = 1.
 *
 *  The first routine finds the magnitude on the largest element in the
 *  matrix.  If the matrix has not yet been factored, the largest
 *  element is found by direct search.  If the matrix is factored, a
 *  bound on the largest element in any of the reduced submatrices is
 *  computed using Barlow with p = oo and q = 1.  The ratio of these
 *  two numbers is the growth, which can be used to determine if the
 *  pivoting order is adequate.  A large growth implies that
 *  considerable error has been made in the factorization and that it
 *  is probably a good idea to reorder the matrix.  If a large growth
 *  in encountered after using spFactor(), reconstruct the matrix and
 *  refactor using spOrderAndFactor().  If a large growth is
 *  encountered after using spOrderAndFactor(), refactor using
 *  spOrderAndFactor() with the pivot threshold increased, say to 0.1.
 *
 *  Using only the size of the matrix as an upper bound on m_ij and
 *  Barlow's bound, the user can estimate the size of the matrix error
 *  terms e_ij using the bound of Erisman and Reid.  The second routine
 *  computes a tighter bound (with more work) based on work by Gear
 *  [3], |e_ij| < 1.01 u rho (t c^3 + (1 + t)c^2) where t is the
 *  threshold and c is the maximum number of off-diagonal elements in
 *  any row of L.  The expensive part of computing this bound is
 *  determining the maximum number of off-diagonals in L, which changes
 *  only when the order of the matrix changes.  This number is computed
 *  and saved, and only recomputed if the matrix is reordered.
 *
 *  [1] A. M. Erisman, J. K. Reid.  Monitoring the stability of the
 *      triangular factorization of a sparse matrix.  Numerische
 *      Mathematik.  Vol. 22, No. 3, 1974, pp 183-186.
 *
 *  [2] J. L. Barlow.  A note on monitoring the stability of triangular
 *      decomposition of sparse matrices.  "SIAM Journal of Scientific
 *      and Statistical Computing."  Vol. 7, No. 1, January 1986, pp 166-168.
 *
 *  [3] I. S. Duff, A. M. Erisman, J. K. Reid.  "Direct Methods for Sparse
 *      Matrices."  Oxford 1986. pp 99.
 */

/*
 *  LARGEST ELEMENT IN MATRIX
 *
 *  >>> Returns: [REAL or DOUBLE PRECISION]
 *  If matrix is not factored, returns the magnitude of the largest element in
 *  the matrix.  If the matrix is factored, a bound on the magnitude of the
 *  largest element in any of the reduced submatrices is returned.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *     Pointer to the matrix.
 */

RealNumber
sfLargestElement( long *Matrix )
{
    /* Begin `sfLargestElement'. */
    return spLargestElement( (char *)Matrix );
}




/*
 *  MATRIX ROUNDOFF ERROR
 *
 *  >>> Returns: [REAL or DOUBLE PRECISION]
 *  Returns a bound on the magnitude of the largest element in E = A - LU.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (long *) [INTEGER]
 *      Pointer to the matrix.
 *  Rho  <input>  (RealNumber *) [REAL or DOUBLE PRECISION]
 *      The bound on the magnitude of the largest element in any of the
 *      reduced submatrices.  This is the number computed by the function
 *      spLargestElement() when given a factored matrix.  If this number is
 *      negative, the bound will be computed automatically.
 */

RealNumber
sfRoundoff( long *Matrix, RealNumber *Rho )
{
    /* Begin `sfRoundoff'. */
    return spRoundoff( (char *) * Matrix, *Rho );
}
#endif

#endif /* FORTRAN */
