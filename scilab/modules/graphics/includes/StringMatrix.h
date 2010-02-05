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
/* File    : StringMatrix.h                                                                  */
/* Desc.   : Allocation, deletion and modifications of matrices of strings.                  */
/*           The matrix is stored by colmuns like in Scilab                                  */
/*----------------------------------------------------------------------------------*/

#ifndef _SCI_STRING_MATRIX_H_
#define _SCI_STRING_MATRIX_H_

#include "dynlib_graphics.h"
#include "sciMatrix.h"

/*----------------------------------------------------------------------------------*/
/**
 * The StringMatrix is just a pointer matrix. So it can be used in any function using
 * sciMatrix. The typedef is used for a more understandable code.
 */
typedef sciMatrix StringMatrix ;

/*----------------------------------------------------------------------------------*/
/* Constructors */
/**
 * create a nbRow x nbCol matrix which data are copied.
 * @param textMat the copied data.
 */
GRAPHICS_IMPEXP StringMatrix * newFullStringMatrix( char ** textMat, int nbRow, int nbCol ) ;

/**
 * Create a new string matrix filled with empty strings
 */
GRAPHICS_IMPEXP StringMatrix * newEmptyStringMatrix(int nbRow, int nbCol);

/** 
 * copy constructor
 */
GRAPHICS_IMPEXP StringMatrix * copyStringMatrix( const StringMatrix * copyMat ) ;

/*----------------------------------------------------------------------------------*/
/* accessors */
GRAPHICS_IMPEXP char * getStrMatElement( const StringMatrix * mat, int row, int col ) ;

/**
 * get the pointer on the array of string. May be used for faster access to the data.
 */
GRAPHICS_IMPEXP char ** getStrMatData(  const StringMatrix * mat ) ;

/**
 * desalocate the (row,col) current string and copy the new one.
 */
GRAPHICS_IMPEXP void copyStrMatElement( StringMatrix * mat, int row, int col, const char * copyStr ) ;

/*----------------------------------------------------------------------------------*/
/* utilities */
/**
 * Print the matrix using printf
 */
GRAPHICS_IMPEXP void printStrMat(StringMatrix * mat);
/*----------------------------------------------------------------------------------*/

#endif /* _SCI_STRING_MATRIX_H_ */
