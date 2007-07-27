/*
**  -*- C -*-
**
** issymmetric.c
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Jul 19 12:12:28 2007 bruno
** Last update Thu Jul 26 16:28:13 2007 bruno
**
** Copyright INRIA 2007
*/

#include "stack-c.h"

#define SYMETRIC	1
#define NOT_SYMETRIC	0

/*
** Check wether or not a Matrix is Symetric.
*/

/** TODO : Check With SERGE. */

/*
**   Two cases :
**   ~ Real Case :
**   --------------
**   - Check that symetrics elements are strictly equals.
**
**   ~ Complex Case :
**   -----------------
**   - Check that symetrics elements are conjugate.
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

  /** If the matrix is not Square, it can not be symmetric */
  if (m != n) {
    return(NOT_SYMETRIC);
  }

  if (isComplex(intAddress)) {
    /*
    ** REAL Case.
    */
    for(lineIterator = 0 ; lineIterator < n ; ++lineIterator) {
      for (columnIterator = 0 ; columnIterator < lineIterator ; ++columnIterator) {
	elementAddress = l + columnIterator + lineIterator * n;
	symetricElementAddress = l + columnIterator * n + lineIterator;
	double rest = abs(getElementByAddress(elementAddress) - getElementByAddress(symetricElementAddress));
	if (rest > 0) {
	  return NOT_SYMETRIC;
	}
      }
    }
    /* We have not detected the Matrix is not Symetric */
    return SYMETRIC;
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
      if (abs(getElementByAddress(symetricElementAddress)) > 0) {
	return NOT_SYMETRIC;
      }
    }
    /** END : Diag Check */

    /** Generic Complex case */
    for (lineIterator = 1 ; lineIterator < n ; ++lineIterator) {
      for (columnIterator = 1 ; columnIterator <= lineIterator ; ++columnIterator) {
	elementAddress = l + columnIterator - 1 + lineIterator * n;
	symetricElementAddress = l + (columnIterator - 1) * n + lineIterator;
	double realRest = abs(getElementByAddress(elementAddress) - getElementByAddress(symetricElementAddress));
	double imagRest = abs(getElementByAddress(elementAddress + size) + getElementByAddress(symetricElementAddress + size));
	if (realRest > 0 || imagRest > 0) {
	  return NOT_SYMETRIC;
	}
      }
    }
    /* We have not detected the Matrix is not Symetric */
    return SYMETRIC;
  }
}
