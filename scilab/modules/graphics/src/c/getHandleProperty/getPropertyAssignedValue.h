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

/*------------------------------------------------------------------------*/
/* file: getPropertyAssignedValue.h                                       */
/* desc : a set of functions used to get the values which will be         */
/*        assigned to handles properties from the stack                   */
/*------------------------------------------------------------------------*/

#ifndef _GET_PROPERTY_ASSIGNEMENT_VALUE_H_
#define _GET_PROPERTY_ASSIGNEMENT_VALUE_H_

#include "machine.h"
#include <stdlib.h> /* for size_t */

/*------------------------------------------------------------------------------*/
/* Basic type parameters */
/**
 * return if a stack parameter is a Handle
 * @param type type of the parameter
 */
BOOL isParameterHandle( int type ) ;

/**
 * return if a stack parameter is a Handle
 * @param type type of the parameter
 */
BOOL isParameterDoubleMatrix( int type ) ;

/**
 * return if a stack parameter is a tlist
 * @param type type of the parameter
 **/
BOOL isParameterTlist( int type ) ;

/**
 * return if a stack parameter is a string matrix
 * @param type type of the parameter
 */
BOOL isParameterStringMatrix( int type ) ;

/**
 * retrieve a single double from the stack
 */
double getDoubleFromStack( size_t stackPointer ) ;

/**
 * retrieve a double matrix from the scilab stack
 */
double * getDoubleMatrixFromStack( size_t stackPointer ) ;

/**
 * copy a double vector from the scilab stack
 */
void copyDoubleVectorFromStack( size_t stackPointer, double dest[], int nbElement ) ;

/**
 * copy a double vector from the scilab stack to an int array
 * with int cast for each parameter.
 */
void copyDoubleVectorToIntFromStack( size_t stackPointer, int dest[], int nbElement ) ;

/**
 * create a copy of a vector (or Matrix) of double stored in the stack
 */
double * createCopyDoubleVectorFromStack( size_t stackPointer, int nbElement ) ;

/**
 * retrieve a string on from the scilab stack
 */
char * getStringFromStack( size_t stackPointer ) ;

/**
 * retrieve a string matrix on from the scilab stack
 */
char ** getStringMatrixFromStack( size_t stackPointer ) ;

/**
 * create a copy of a stringMatrix which is in the stack
 */
char ** createCopyStringMatrixFromStack( size_t stackPointer, int nbElement ) ;

/**
 * retrieve a handle on from the Scilab stack
 */
unsigned long getHandleFromStack( size_t stackPointer ) ;

/**
 * compare the string stored in the stack with str
 * @return TRUE is the string are equal (using strcmp)
 *         FALSE otherwise
 */
BOOL isStringParamEqual( size_t stackPointer, const char * str ) ;
/*------------------------------------------------------------------------------*/
/* Tlist */
typedef struct 
{
  int nbElement ; /**< number of elements in the tlist */
  int curElement ; /**< currently read element */
  int paramNumber ; /**< rank of the tlist within the Rhs parameters */
  size_t stackPointer ; /**< pointer of the tlist in the stack */
} AssignedList ;

/**
 * get the number of element of a tlist stored in the rhs
 * @param paramNum rank of the list within the Rhs parameters
 */
int getStackListNbElement( int paramNum ) ;

/**
 * create a new instance of an object used to retrieve fields of a tlist
 * stored on the stack
 * @param paramNum rank of the list within the Rhs parameters
 * @param nbElement number of element in the list
 */
AssignedList * createAssignedList( int paramNum, int nbElement ) ;

/**
 * destroy the object used to glance through a tlist
 */
void destroyAssignedList( AssignedList * list ) ;

/**
 * return the number of element of a tlist
 */
int getAssignedListNbElement( AssignedList * list ) ;

/**
 * set the current element to the first
 */
void rewindAssignedList( AssignedList * list ) ;

/**
 * Return whether the current element of the list is a double matrix or not.
 */
BOOL isListCurrentElementDoubleMatrix( AssignedList * list ) ;

/**
* Return whether the current element of the list is a string matrix or not.
*/
BOOL isListCurrentElementStringMatrix( AssignedList * list ) ;

/**
* Return whether the current element of the list is an empty matrix or not.
*/
BOOL isListCurrentElementEmptyMatrix( AssignedList * list ) ;

/**
 * retrieve a field of a tlist
 * @param[in]  list object used to retrieve elements
 * @param      rank position of the element in the list ( first, second, ...)
 *             Note that is it not possible to get the properties names with this function
 * @param[out] nbRow number of row of the returned matrix
 * @param[out] nbCol number of column of the returned matrix
 */
double * getDoubleMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol ) ;

/**
* retrieve a field of a tlist
* @param[in]  list object used to retrieve elements
* @param      rank position of the element in the list ( first, second, ...)
*             Note that is it not possible to get the properties names with this function
* @param[out] nbRow number of row of the returned matrix
* @param[out] nbCol number of column of the returned matrix
*/
char ** getStringMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol ) ;


/**
 * retrieve the current property of a tlist and move to the next
 * @param[in/out] list object used to retrieve elements
 * @param[out]    nbRow number of row of the returned matrix
 * @param[out]    nbCol number of column of the returned matrix
 */
double * getCurrentDoubleMatrixFromList( AssignedList * list, int * nbRow, int * nbCol ) ;

/**
* retrieve the current property of a tlist and move to the next
* @param[in/out] list object used to retrieve elements
* @param[out]    nbRow number of row of the returned matrix
* @param[out]    nbCol number of column of the returned matrix
*/
char ** getCurrentStringMatrixFromList( AssignedList * list, int * nbRow, int * nbCol ) ;

/**
 * create a copy of the current matrix in the tlist
 * @param[in/out] list object used to retrieve elements
 * @param[out]    nbRow number of row of the returned matrix or -1 if an error occurred
 * @param[out]    nbCol number of column of the returned matrix or -1 if an error occurred
 * @return the created array or NULL if the matrix is empty or an error occured
 */
double * createCopyDoubleMatrixFromList( AssignedList * list, int * nbRow, int * nbCol ) ;

/**
* create a copy of the current matrix in the tlist
* @param[in/out] list object used to retrieve elements
* @param[out]    nbRow number of row of the returned matrix
* @param[out]    nbCol number of column of the returned matrix
*/
char ** createCopyStringMatrixFromList( AssignedList * list, int * nbRow, int * nbCol ) ;

/*------------------------------------------------------------------------------*/
#endif /* _GET_PROPERTY_ASSIGNEMENT_VALUE_H_ */
