/*----------------------------------------------------------------------------------*/
/* COPYRIGHT INRIA 2006                                                                      */
/* File    : Vect2Matrix.h                                                                   */
/* Authors : Jean-Baptiste Silvy 2006-xxxx                                                   */
/* Desc.   : Allocation and deletion and modifications of matrices of 2d vector.             */
/*           The matrix stored arrays of size two of int or double                           */
/*----------------------------------------------------------------------------------*/

#ifndef _SCI_VECT2_MATRIX_H_
#define _SCI_VECT2_MATRIX_H_

#include "sciMatrix.h"

/*----------------------------------------------------------------------------------*/
/* matrix of int[2] */
typedef sciMatrix Vect2iMatrix ;

/* accessors */
int * getVect2iMatElement( const Vect2iMatrix * mat, int row, int col ) ;

/* desalocate the (row,col) current string and copy the new one. */
void copyVect2iMatElement(       Vect2iMatrix * mat,
                                 int row           ,
                                 int col           , 
                           const int copyValue[2]   ) ;

/* rotate all the element in the matrix of a given angle around the center */
void rotateVect2iMatrix( Vect2iMatrix * mat, int center[2], double angle ) ;

/**
 * translate a matrix of vector trans.
 */
void translateVect2iMatrix( Vect2iMatrix * mat, int trans[2] ) ;

/* apply an homothety on all element */
void homothVect2iMatrix( Vect2iMatrix * mat, int center[2], double factors[2] ) ;

/*----------------------------------------------------------------------------------*/
/* matrix of double[2] */
typedef sciMatrix Vect2dMatrix ;

/* accessors */
double * getVect2dMatElement( const Vect2dMatrix * mat, int row, int col ) ;

/* desalocate the (row,col) current string and copy the new one. */
void copyVect2dMatElement(       Vect2dMatrix * mat,
                                 int    row        ,
                                 int    col        , 
                           const double copyValue[2] ) ;

/**
 * translate a matrix of vector trans.
 */
void translateVect2dMatrix( Vect2dMatrix * mat, double trans[2] ) ;

/* rotate all the element in the matrix of a given angle around the center */
void rotateVect2dMatrix( Vect2dMatrix * mat, double center[2], double angle ) ;

/* apply an homothety on all element */
void homothVect2dMatrix( Vect2dMatrix * mat, double center[2], double factors[2] ) ;


/*----------------------------------------------------------------------------------*/

#endif /* _SCI_VECT2_MATRIX_H_ */
