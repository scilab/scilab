
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno Jofret <bruno.jofret@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
/*
**  -*- C -*-
**
** issymmetric.c
**
** Started on  Thu Jul 19 12:12:28 2007 bruno
** Last update Fri Mar 13 15:48:48 2009 Bernard Hugueney -> use iIsComplex from stack3.h 
*/

#include "issymmetric.h"
#include "stack3.h"
/*
** Check wether or not a Matrix is Symmetric.
*/

/*
**   Two cases :
**   ~ Real Case :
**   --------------
**   - Check that symmetrics elements are strictly equals.
**
**   ~ Complex Case :
**   -----------------
**   - Check that symmetrics elements are conjugate.
**   - Check that diagonal's elements are Real.
*/


int C2F(issymmetric)(int *stackPosition) {

  int relativePosition = Top - Rhs + *stackPosition;
  int address = *Lstk(relativePosition);
  int intAddress = iadr(address);
  int m = getNumberOfLines(intAddress);
  int n = getNumberOfColumns(intAddress);
  int l = getDoubleDataAddress(intAddress);
  int size = m * n;

  /* Local variables */
  int lineIterator = 0;
  int columnIterator = 0;
  int elementAddress = 0;
  int symetricElementAddress = 0;
  double realRest = 0;
  double imagRest = 0;

  /** If the matrix is not Square, it can not be symmetric */
  if (m != n) {
    return(NOT_SYMMETRIC);
  }

  if (!iIsComplex(*stackPosition)) {
    /*
    ** REAL Case.
    */
    for(lineIterator = 0 ; lineIterator < n ; ++lineIterator) {
      for (columnIterator = 0 ; columnIterator < lineIterator ; ++columnIterator) {
	elementAddress = l + columnIterator + lineIterator * n;
	symetricElementAddress = l + columnIterator * n + lineIterator;
	realRest = fabs(getElementByAddress(elementAddress) - getElementByAddress(symetricElementAddress));
	if (realRest > 0) {
	  return NOT_SYMMETRIC;
	}
      }
    }
    /* We have not detected the Matrix is not Symetric */
    return SYMMETRIC;
  }
  else {
    /*
    ** COMPLEX case.
    */

    /*
    ** Just Checking that diags are completely real
    ** i.e their imaginary part is 0.
    */
    /** { BEGIN : Diag check */
    for(lineIterator = 0 ; lineIterator < n ; ++lineIterator) {
      elementAddress = l + lineIterator + lineIterator * n;
      symetricElementAddress = elementAddress + size;
      if (fabs(getElementByAddress(symetricElementAddress)) > 0) {
	return NOT_SYMMETRIC;
      }
    }
    /** END : Diag Check */

    /** Generic Complex case */
    for (lineIterator = 1 ; lineIterator < n ; ++lineIterator) {
      for (columnIterator = 1 ; columnIterator <= lineIterator ; ++columnIterator) {
	elementAddress = l + columnIterator - 1 + lineIterator * n;
	symetricElementAddress = l + (columnIterator - 1) * n + lineIterator;
	realRest = fabs(getElementByAddress(elementAddress) - getElementByAddress(symetricElementAddress));
	imagRest = fabs(getElementByAddress(elementAddress + size) + getElementByAddress(symetricElementAddress + size));
	if (realRest > 0 || imagRest > 0) {
	  return NOT_SYMMETRIC;
	}
      }
    }
    /* We have not detected the Matrix is not Symetric */
    return SYMMETRIC;
  }
}
