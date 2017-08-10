/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
typedef sciMatrix StringMatrix;

/*----------------------------------------------------------------------------------*/
/* Constructors */
/**
 * create a nbRow x nbCol matrix which data are copied.
 * @param textMat the copied data.
 */
GRAPHICS_IMPEXP StringMatrix * newFullStringMatrix(char ** textMat, int nbRow, int nbCol);

/**
 * Create a new string matrix filled with empty strings
 */
GRAPHICS_IMPEXP StringMatrix * newEmptyStringMatrix(int nbRow, int nbCol);

/**
 * copy constructor
 */
GRAPHICS_IMPEXP StringMatrix * copyStringMatrix(const StringMatrix * copyMat);

/*----------------------------------------------------------------------------------*/
/* accessors */
GRAPHICS_IMPEXP char * getStrMatElement(const StringMatrix * mat, int row, int col);

/**
 * get the pointer on the array of string. May be used for faster access to the data.
 */
GRAPHICS_IMPEXP char ** getStrMatData( const StringMatrix * mat);

/**
 * desalocate the (row,col) current string and copy the new one.
 */
GRAPHICS_IMPEXP void copyStrMatElement(StringMatrix * mat, int row, int col, const char * copyStr);

/*----------------------------------------------------------------------------------*/
/* utilities */
/**
 * Print the matrix using printf
 */
GRAPHICS_IMPEXP void printStrMat(StringMatrix * mat);
/*----------------------------------------------------------------------------------*/

#endif /* _SCI_STRING_MATRIX_H_ */
