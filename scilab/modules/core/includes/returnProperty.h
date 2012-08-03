/*
 * Scilab (http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
/* file: returnProperty.h                                                 */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#ifndef _RETURN_PROPERTY_H_
#define _RETURN_PROPERTY_H_

/**
 * create an empty matrix in the scilab stack in order to see it in the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnEmptyMatrix(void* _pvCtx) ;

/**
 * copy the string value in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnString(void* _pvCtx, const char * value ) ;

/**
 * copy a char value into a scilab string in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnChar(void* _pvCtx, char value ) ;

/**
 * copy a single double in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnDouble(void* _pvCtx, double value ) ;

/**
 * copy a single int in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnInt(void* _pvCtx, int value ) ;

/**
 * copy a double array into a row vector in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnRowVector(void* _pvCtx, const double values[], int nbValues ) ;

/**
* copy a int array into a row vector (of double) in the scilab stack
* in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occurred.
*/
int sciReturnRowVectorFromInt(void* _pvCtx, const int values[], int nbValues ) ;

/**
 * copy an int array into an int row vector in the scilab stack
 * in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnRowIntVector(void* _pvCtx, const int values[], int nbValues ) ;

/**
 * copy a char * array into an string row vector in the scilab stack
 * in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnRowStringVector(void* _pvCtx, char * values[], int nbValues ) ;

/**
 * copy a handle in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnHandle(void* _pvCtx, long handle ) ;

/**
 * copy an handle array into a row vector in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnRowHandleVector(void* _pvCtx, const long handles[], int nbValues ) ;

/**
* copy an handle array into a col vector in the scilab stack in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occurred.
*/
int sciReturnColHandleVector(void* _pvCtx, const long handles[], int nbValues ) ;

/**
 * copy a matrix stored has a double array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnMatrix(void* _pvCtx, double values[], int nbRow, int nbCol ) ;

/**
 * copy a matrix stored has a char * array
 * into a Scilab string matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnStringMatrix(void* _pvCtx, char * values[], int nbRow, int nbCol ) ;

/**
 * return user data in the scilab console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
int sciReturnUserData(void* _pvCtx, const int * userData, int userDataSize ) ;

#endif /* _RETURN_PROPERTY_H_ */
