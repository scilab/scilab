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
/* Desc.   : Allocation and deletion and modifications of matrices of strings.               */
/*           The matrix is stored by colmuns like in Scilab                                  */
/*----------------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "StringMatrix.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

/*----------------------------------------------------------------------------------*/
StringMatrix * newFullStringMatrix( char ** textMat, int nbRow, int nbCol )
{
  int i = 0;
  /* create the matrix */
  StringMatrix * newMat = newMatrix( nbRow, nbCol ) ;

  /* copy each element */
  for ( i = 0 ; i < nbRow * nbCol ; i++ )
  {
    /* +1 for the /0 last character */
    newMat->data[i] = MALLOC( (strlen(textMat[i]) + 1) * sizeof(char) ) ;
    strcpy( newMat->data[i], textMat[i] ) ;
  }

  return newMat ;
}
/*----------------------------------------------------------------------------------*/
StringMatrix * newEmptyStringMatrix(int nbRow, int nbCol)
{
  int i = 0;
  /* create the matrix */
  StringMatrix * newMat = newMatrix( nbRow, nbCol ) ;

  /* copy each element */
  for ( i = 0 ; i < nbRow * nbCol ; i++ )
  {
    /* +1 for the /0 last character */
    newMat->data[i] = MALLOC( (strlen("") + 1) * sizeof(char) ) ;
    strcpy( newMat->data[i], "" ) ;
  }

  return newMat ;
}
/*----------------------------------------------------------------------------------*/
StringMatrix * copyStringMatrix( const StringMatrix * copyMat )
{
  return newFullStringMatrix( (char **) copyMat->data, copyMat->nbRow, copyMat->nbCol ) ;
}
/*----------------------------------------------------------------------------------*/
char * getStrMatElement( const StringMatrix * mat, int row, int col )
{
  return (char *) getMatElement( mat, row, col ) ;
}
/*----------------------------------------------------------------------------------*/
char ** getStrMatData( const StringMatrix * mat )
{
  return (char **) getMatData( mat ) ;
}
/*----------------------------------------------------------------------------------*/
void copyStrMatElement( StringMatrix * mat, int row, int col, const char * copyStr )
{
  char * changedString = (char *) mat->data[row + col * mat->nbRow] ; /* for speed */
  if ( changedString != NULL )
  {
    FREE( changedString ) ;
  }
  changedString = strdup(copyStr);
  mat->data[row + col * mat->nbRow] = changedString ;
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
