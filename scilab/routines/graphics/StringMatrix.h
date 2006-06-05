/*-------------------------------------------------------------------------------------------*/
/* COPYRIGHT INRIA 2006                                                                      */
/* File    : StringMatrix.h                                                                  */
/* Authors : Jean-Baptiste Silvy 2006-xxxx                                                   */
/* Desc.   : Allocation, deletion and modifications of matrices of strings.                  */
/*           The matrix is stored by colmuns like in Scilab                                  */
/*-------------------------------------------------------------------------------------------*/

#ifndef _SCI_STRING_MATRIX_H_
#define _SCI_STRING_MATRIX_H_

#include "sciMatrix.h"

/*-------------------------------------------------------------------------------------------*/
/* matrix of strings */
typedef sciMatrix StringMatrix ;

/*-------------------------------------------------------------------------------------------*/
/* Constructors */
/* create a nbRow x nbCol matrix which data are copied from textMat */
StringMatrix * newFullStringMatrix( char ** textMat, int nbRow, int nbCol ) ;

/* copy constructor */
StringMatrix * copyStringMatrix( const StringMatrix * copyMat ) ;

/*-------------------------------------------------------------------------------------------*/
/* accessors */
char * getStrMatElement( const StringMatrix * mat, int row, int col ) ;

char ** getStrMatData(  const StringMatrix * mat ) ;

/* desalocate the (row,col) current string and copy the new one */
void copyStrMatElement( StringMatrix * mat, int row, int col, const char * copyStr ) ;

/*-------------------------------------------------------------------------------------------*/

#endif /* _SCI_STRING_MATRIX_H_ */
