/*------------------------------------------------------------------------*/
/* file: getPropertyAssignedValue.h                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to get the values which will be         */
/*        assigned to handles properties from the stack                   */
/*------------------------------------------------------------------------*/

#ifndef _GET_PROPERTY_ASSIGNEMENT_VALUE_H_
#define _GET_PROPERTY_ASSIGNEMENT_VALUE_H_

#include "machine.h"

/**
 * return if a stack parameter is a Handle
 * @param paramNum Rank of the parameter in the called primitive
 */
BOOL isParameterHandle( int paramNum ) ;

/**
* return if a stack parameter is a Handle
* @param paramNum Rank of the parameter in the called primitive
*/
BOOL isParameterDoubleMatrix( int paramNum ) ;

/**
 * retrieve a single double from the stack
 */
double getDoubleFromStack( int stackPointer ) ;

/**
 * retrieve a double matrix from the scilab stack
 */
double * getDoubleMatrixFromStack( int stackPointer ) ;

/**
 * retireve a string on from the scilab stack
 */
char * getStringFromStack( int stackPointer ) ;

/**
* retireve a string on from the scilab stack
*/
long getHandleFromStack( int stackPointer ) ;

/**
 * compare the string stored in the stack with str
 * @return TRUE is the string are equal (using strcmp)
 *         FALSE otherwise
 */
BOOL isStringParamEqual( int stackPointer, const char * str ) ;

#endif /* _GET_PROPERTY_ASSIGNEMENT_VALUE_H_ */
