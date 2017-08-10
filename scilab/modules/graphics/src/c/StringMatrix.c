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
/* COPYRIGHT INRIA 2006                                                                      */
/* Desc.   : Allocation and deletion and modifications of matrices of strings.               */
/*           The matrix is stored by colmuns like in Scilab                                  */
/*----------------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "StringMatrix.h"
#include "sci_malloc.h"
#include "os_string.h"


/*----------------------------------------------------------------------------------*/
StringMatrix * newFullStringMatrix(char ** textMat, int nbRow, int nbCol)
{
    int i = 0;
    /* create the matrix */
    StringMatrix * newMat = newMatrix(nbRow, nbCol);

    /* copy each element */
    for (i = 0 ; i < nbRow * nbCol ; i++)
    {
        /* +1 for the /0 last character */
        newMat->data[i] = MALLOC((strlen(textMat[i]) + 1) * sizeof(char));
        strcpy(newMat->data[i], textMat[i]);
    }

    return newMat;
}
/*----------------------------------------------------------------------------------*/
StringMatrix * newEmptyStringMatrix(int nbRow, int nbCol)
{
    int i = 0;
    /* create the matrix */
    StringMatrix * newMat = newMatrix(nbRow, nbCol);

    /* copy each element */
    for (i = 0 ; i < nbRow * nbCol ; i++)
    {
        /* +1 for the /0 last character */
        newMat->data[i] = MALLOC((strlen("") + 1) * sizeof(char));
        strcpy(newMat->data[i], "");
    }

    return newMat;
}
/*----------------------------------------------------------------------------------*/
StringMatrix * copyStringMatrix(const StringMatrix * copyMat)
{
    return newFullStringMatrix((char **) copyMat->data, copyMat->nbRow, copyMat->nbCol);
}
/*----------------------------------------------------------------------------------*/
char * getStrMatElement(const StringMatrix * mat, int row, int col)
{
    return (char *) getMatElement(mat, row, col);
}
/*----------------------------------------------------------------------------------*/
char ** getStrMatData(const StringMatrix * mat)
{
    return (char **) getMatData(mat);
}
/*----------------------------------------------------------------------------------*/
void copyStrMatElement(StringMatrix * mat, int row, int col, const char * copyStr)
{
    char * changedString = (char *) mat->data[row + col * mat->nbRow] ; /* for speed */
    if (changedString != NULL)
    {
        FREE(changedString);
    }
    changedString = os_strdup(copyStr);
    mat->data[row + col * mat->nbRow] = changedString;
}
/*----------------------------------------------------------------------------------*/
void printStrMat(StringMatrix * mat)
{
    int i = 0;
    int nbRow = getMatNbRow(mat);
    int nbCol = getMatNbCol(mat);
    for (i = 0; i < nbRow; i++)
    {
        int j = 0;
        for (j = 0; j < nbCol; j++)
        {
            printf("%s ", getStrMatElement(mat, i, j));
        }
        printf("\n");
    }
}
/*----------------------------------------------------------------------------------*/
