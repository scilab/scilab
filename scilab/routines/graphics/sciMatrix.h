/*-------------------------------------------------------------------------------------------*/
/* COPYRIGHT INRIA 2006                                                                      */
/* File    : sciMatrix.h                                                                     */
/* Authors : Jean-Baptiste Silvy 2006-xxxx                                                   */
/* Desc.   : Allocation and deletion and modifications of matrices of pointers               */
/*           The matrix is stored by colmuns like in Scilab.                                 */
/*           These matrices can be used as generic matrices since they used void * pointers  */
/*-------------------------------------------------------------------------------------------*/

#ifndef _SCI_MATRIX_H_
#define _SCI_MATRIX_H_

/*-------------------------------------------------------------------------------------------*/
/* matrix of strings */
typedef struct
{
  void ** data ;
  int nbCol    ;
  int nbRow    ;
}
sciMatrix ;

/*-------------------------------------------------------------------------------------------*/
/* Constructors */
/* allocate a matrix with no elements (nbrow = nbcol = 0) */
sciMatrix * emptyMatrix( void ) ;

/* create a nbRow x nbCol matrix of NULL pointers */
sciMatrix * newMatrix( int nbRow, int nbCol ) ;

/* create a nbRow x nbCol matrix which data are dataMat (directly, no copy). */
sciMatrix * newCompleteMatrix( void ** dataMat, int nbRow, int nbCol ) ;

/* note that we cannot use a copy constructor since we don't know how to copy two elements */
/* of the matrix! Maybe it is possible with some function pointers, but it seems a bit */
/* tricky for me. For something clean, C++ may be better. */
/*-------------------------------------------------------------------------------------------*/
/* destructor */
/* delete the structure and data */
void deleteMatrix( sciMatrix * mat ) ;

/* delete only the structure, not the data (use with caution) */
void desallocateMatrix( sciMatrix * mat ) ;

/*-------------------------------------------------------------------------------------------*/
/* accessors */
void * getMatElement( const sciMatrix * mat, int row, int col ) ;

int     getMatNbRow( const sciMatrix * mat ) ;

int     getMatNbCol( const sciMatrix * mat ) ;

void ** getMatData(  const sciMatrix * mat ) ;

/* set the element (row,col) of the matrix to newStr, but does not desalocate the previous */
/* if one exists. Does not copy the string also. */
void setMatElement(    sciMatrix * mat, int row, int col, void * newValue ) ;

/* desalocate the (row,col) current string and set the new one */
void changeMatElement( sciMatrix * mat, int row, int col, void * newValue ) ;

/* desalocate the (row,col) current element (i,j) and copy the new one. The size of the element
   must be given in order to allocate memory. */
void copyMatElement(       sciMatrix * mat      ,
                           int             row      ,
                           int             col      , 
                     const void          * copyValue,
                           int             valueSize ) ;

/*-------------------------------------------------------------------------------------------*/

#endif /* _SCI_MATRIX_H_ */
