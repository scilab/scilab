/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------------*/
/* COPYRIGHT INRIA 2006                                                                      */
/* Desc.   : Allocation and deletion and modifications of matrices of 2d vector.             */
/*           The matrix stored arrays of size two of int or double                           */
/*----------------------------------------------------------------------------------*/

#include "Vect2Matrix.h"
#include "math_graphics.h"

/*----------------------------------------------------------------------------------*/
int * getVect2iMatElement( const Vect2iMatrix * mat, int row, int col )
{
  return (int *) getMatElement( mat, row, col ) ;
}
/*----------------------------------------------------------------------------------*/
void copyVect2iMatElement(       sciMatrix * mat,
                                 int row        ,
                                 int col        , 
                           const int copyValue[2] )
{
  copyMatElement( mat, row, col, copyValue, 2 * sizeof(int) ) ;
}
/*----------------------------------------------------------------------------------*/
void rotateVect2iMatrix( Vect2iMatrix * mat, int center[2], double angle )
{
  double cosAngle = cos( angle ) ;
  double sinAngle = sin( angle ) ;
  int i ;
  int j ;
  for ( i = 0 ; i < mat->nbRow; i++ )
  {
    for ( j = 0 ; j < mat->nbCol ; j++ )
    {
      int * curElement = getVect2iMatElement( mat, i, j ) ;
      iRotate2Dim( curElement, center, cosAngle, sinAngle, curElement ) ;
    }
  }
}
/*----------------------------------------------------------------------------------*/
void translateVect2iMatrix( Vect2iMatrix * mat, int trans[2] )
{
  int i ;
  int j ;
  for ( i = 0 ; i < mat->nbRow ; i++ )
  {
    for ( j = 0 ; j < mat->nbCol ; j++ )
    {
      int * curElement = getVect2iMatElement( mat, i, j ) ;
      iTranslate2D( curElement, trans, curElement ) ;
    }
  }
}
/*----------------------------------------------------------------------------------*/
void homothVect2iMatrix( Vect2iMatrix * mat, int center[2], double factors[2] )
{
  int i ;
  int j ;
  for ( i = 0 ; i < mat->nbRow; i++ )
  {
    for ( j = 0 ; j < mat->nbCol ; j++ )
    {
      int * curElement = getVect2iMatElement( mat, i, j ) ;
      iHomothety2D( curElement, center, factors, curElement ) ;
    }
  }
}
/*----------------------------------------------------------------------------------*/
double * getVect2dMatElement( const Vect2dMatrix * mat, int row, int col )
{
  return (double *) getMatElement( mat, row, col ) ;
}
/*----------------------------------------------------------------------------------*/
void copyVect2dMatElement(       Vect2dMatrix * mat,
                                 int    row        ,
                                 int    col        , 
                           const double copyValue[2] )
{
  copyMatElement( mat, row, col, copyValue, 2 * sizeof(double) ) ;
}
/*----------------------------------------------------------------------------------*/
void rotateVect2dMatrix( Vect2dMatrix * mat, double center[2], double angle )
{
  double cosAngle = cos( angle ) ;
  double sinAngle = sin( angle ) ;
  int i ;
  int j ;
  for ( i = 0 ; i < mat->nbRow ; i++ )
  {
    for ( j = 0 ; j < mat->nbCol ; j++ )
    {
      double * curElement = getVect2dMatElement( mat, i, j ) ;
      rotate2Dim( curElement, center, cosAngle, sinAngle, curElement ) ;
    }
  }
}
/*----------------------------------------------------------------------------------*/
void translateVect2dMatrix( Vect2dMatrix * mat, double trans[2] )
{
  int i ;
  int j ;
  for ( i = 0 ; i < mat->nbRow ; i++ )
  {
    for ( j = 0 ; j < mat->nbCol ; j++ )
    {
      double * curElement = getVect2dMatElement( mat, i, j ) ;
      translate2D( curElement, trans, curElement ) ;
    }
  }
}
/*----------------------------------------------------------------------------------*/
void homothVect2dMatrix( Vect2dMatrix * mat, double center[2], double factors[2] )
{
  int i ;
  int j ;
  for ( i = 0 ; i < mat->nbRow; i++ )
  {
    for ( j = 0 ; j < mat->nbCol ; j++ )
    {
      double * curElement = getVect2dMatElement( mat, i, j ) ;
      homothety2D( curElement, center, factors, curElement ) ;
    }
  }
}
/*----------------------------------------------------------------------------------*/
