/*----------------------------------------------------------------------------------*/
/* COPYRIGHT INRIA 2006                                                                      */
/* File    : StringMatrix.c                                                                  */
/* Authors : Jean-Baptiste Silvy 2006-xxxx                                                   */
/* Desc.   : Allocation and deletion and modifications of matrices of strings.               */
/*           The matrix is stored by colmuns like in Scilab                                  */
/*----------------------------------------------------------------------------------*/

#include <string.h>
#include "StringMatrix.h"
#include "MALLOC.h"
#include <stdlib.h>

/*----------------------------------------------------------------------------------*/
StringMatrix * newFullStringMatrix( char ** textMat, int nbRow, int nbCol )
{
  int i ;
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
StringMatrix * copyStringMatrix( const StringMatrix * copyMat )
{
  return newFullStringMatrix( (char **) copyMat->data, copyMat->nbCol, copyMat->nbRow ) ;
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
  changedString = MALLOC( (strlen( copyStr ) + 1) * sizeof(char) ) ;
  strcpy( changedString, copyStr ) ;
  mat->data[row + col * mat->nbRow] = changedString ;
}
/*----------------------------------------------------------------------------------*/
