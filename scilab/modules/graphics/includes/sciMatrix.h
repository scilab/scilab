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
/* File    : sciMatrix.h                                                                     */
/* Desc.   : Allocation and deletion and modifications of matrices of pointers               */
/*           The matrix is stored by colmuns like in Scilab.                                 */
/*           These matrices can be used as generic matrices since they used void * pointers  */
/*----------------------------------------------------------------------------------*/

#ifndef _SCI_MATRIX_H_
#define _SCI_MATRIX_H_

#include "dynlib_graphics.h"

/*----------------------------------------------------------------------------------*/
/**
 * a matrix on void pointers
 */
typedef struct
{
    void ** data ; /**< the data are stored in an array of void pointers */
    int nbCol   ;
    int nbRow   ;
}
sciMatrix;

/*----------------------------------------------------------------------------------*/
/* Constructors */
/*@{*/
/**
 * allocate a matrix with no elements (nbrow = nbcol = 0)
 */
GRAPHICS_IMPEXP sciMatrix * emptyMatrix(void);

/**
 * create a nbRow x nbCol matrix of NULL pointers.
 */
GRAPHICS_IMPEXP sciMatrix * newMatrix(int nbRow, int nbCol);

/**
 * create a nbRow x nbCol matrix which data are dataMat (directly, no copy).
 */
GRAPHICS_IMPEXP sciMatrix * newCompleteMatrix(void ** dataMat, int nbRow, int nbCol);
/*@}*/
/* note that we cannot use a copy constructor since we don't know how to copy two elements */
/* of the matrix! Maybe it is possible with some function pointers, but it seems a bit */
/* tricky for me. For something clean, C++ may be better. */
/*----------------------------------------------------------------------------------*/
/* destructor */
/*@{*/
/**
 * delete the structure and data
 */
GRAPHICS_IMPEXP void deleteMatrix(sciMatrix * mat);

/**
 * delete only the structure, not the data (use with caution).
 */
GRAPHICS_IMPEXP void desallocateMatrix(sciMatrix * mat);
/*@}*/
/*----------------------------------------------------------------------------------*/
/* accessors */
/*@{*/
/**
 * retrieve the element (row,col) of the matrix.
 */
GRAPHICS_IMPEXP void * getMatElement(const sciMatrix * mat, int row, int col);

GRAPHICS_IMPEXP int     getMatNbRow(const sciMatrix * mat);

GRAPHICS_IMPEXP int     getMatNbCol(const sciMatrix * mat);

/**
 * get the pointer on the array of data. May be used for faster access to the data.
 */
GRAPHICS_IMPEXP void ** getMatData( const sciMatrix * mat);

/**
 * set an element of the matrix to a new value but does not desalocate the previous
 * if one exists.
 * @param newValue the new value which will be inserted directly in the matrix (no copy).
 */
GRAPHICS_IMPEXP void setMatElement(   sciMatrix * mat, int row, int col, void * newValue);

/**
 * desalocate the (row,col) element and put a new one.
 * @param newValue the new value which will be inserted directly in the matrix (no copy).
 */
GRAPHICS_IMPEXP void changeMatElement(sciMatrix * mat, int row, int col, void * newValue);

/**
 * desalocate the (row,col) current element (i,j) and copy the new one. The size of the element
 * must be given in order to allocate memory.
 * @param copyValue copied value.
 * @param valueSize size of the data inserted in the matrix (ex: sizeof(double)).
 */
GRAPHICS_IMPEXP void copyMatElement(      sciMatrix * mat      ,
        int             row      ,
        int             col      ,
        const void          * copyValue,
        int             valueSize);
/*@}*/
/*----------------------------------------------------------------------------------*/

#endif /* _SCI_MATRIX_H_ */
