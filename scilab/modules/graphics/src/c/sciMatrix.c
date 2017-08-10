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
/* File    : sciMatrix.c                                                                     */
/* Desc.   : Allocation and deletion and modifications of matrices of pointers               */
/*           The matrix is stored by colmuns like in Scilab.                                 */
/*           These matrices acan be used as generic matrices since they used void * pointers */
/*----------------------------------------------------------------------------------*/

#include <string.h>
#include "sciMatrix.h"
#include "sci_malloc.h"

/*----------------------------------------------------------------------------------*/
sciMatrix * emptyMatrix(void)
{
    sciMatrix * newMat;
    newMat = MALLOC(sizeof(sciMatrix));
    if (newMat == NULL)
    {
        return NULL ;
    }
    newMat->data  = NULL;
    newMat->nbCol = 0   ;
    newMat->nbRow = 0   ;

    return newMat;
}
/*----------------------------------------------------------------------------------*/
sciMatrix * newMatrix(int nbRow, int nbCol)
{
    int i = 0;
    /* create the matrix */
    sciMatrix * newMat = emptyMatrix();

    /* allocate the data */
    newMat->data  = MALLOC((nbRow * nbCol) * sizeof(void *));
    newMat->nbRow = nbRow;
    newMat->nbCol = nbCol;

    /* initialize to NULL */
    for (i = 0 ; i < nbRow * nbCol ; i++)
    {
        newMat->data[i] = NULL;
    }

    return newMat;
}
/*----------------------------------------------------------------------------------*/
sciMatrix * newCompleteMatrix(void ** dataMat, int nbRow, int nbCol)
{
    /* create the matrix */
    sciMatrix * newMat = emptyMatrix();

    newMat->data  = dataMat;
    newMat->nbRow = nbRow  ;
    newMat->nbCol = nbCol  ;

    return newMat;
}
/*----------------------------------------------------------------------------------*/
void deleteMatrix(sciMatrix * mat)
{
    int i = 0;
    for (i = 0 ; i < mat->nbRow * mat->nbCol ; i++)
    {
        FREE(mat->data[i]);
        mat->data[i] = NULL;
    }
    FREE(mat->data);
    mat->data = NULL;

    mat->nbCol = 0;
    mat->nbRow = 0;

    FREE(mat);
}
/*----------------------------------------------------------------------------------*/
void desallocateMatrix(sciMatrix * mat)
{
    mat->nbCol = 0;
    mat->nbRow = 0;
    mat->data  = NULL;
    FREE(mat);
}
/*----------------------------------------------------------------------------------*/
void * getMatElement(const sciMatrix * mat, int row, int col)
{
    /* the matrix is stored by column like in scilab */

    return mat->data[row + col * mat->nbRow];
}
/*----------------------------------------------------------------------------------*/
void setMatElement(sciMatrix * mat, int row, int col, void * newValue)
{
    mat->data[row + col * mat->nbRow] = newValue;
}
/*----------------------------------------------------------------------------------*/
void changeMatElement(sciMatrix * mat, int row, int col, void * newValue)
{
    if (mat->data[row + col * mat->nbRow] != NULL)
    {
        FREE(mat->data[row + col * mat->nbRow]);
    }
    mat->data[row + col * mat->nbRow] = newValue;
}
/*----------------------------------------------------------------------------------*/
void copyMatElement(      sciMatrix     * mat      ,
                          int             row      ,
                          int             col      ,
                          const void          * copyValue,
                          int             valueSize)
{
    /* copy the value */
    void * newValue = MALLOC(valueSize);
    memcpy(newValue, copyValue, valueSize);

    /* change the current one */
    changeMatElement(mat, row, col, newValue);
}
/*----------------------------------------------------------------------------------*/
int getMatNbRow(const sciMatrix * mat)
{
    return mat->nbRow ;
}
/*----------------------------------------------------------------------------------*/
int getMatNbCol(const sciMatrix * mat)
{
    return mat->nbCol ;
}
/*----------------------------------------------------------------------------------*/
void ** getMatData(const sciMatrix * mat)
{
    return mat->data ;
}
/*----------------------------------------------------------------------------------*/
