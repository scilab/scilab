/*
 * Scilab (http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
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
/*------------------------------------------------------------------------*/
/* file: returnProperty.h                                                 */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#ifndef _RETURN_PROPERTY_H_
#define _RETURN_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"


/**
 * create an empty matrix in the scilab stack in order to see it in the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnEmptyMatrix(void);

/**
 * copy the string value in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnString(const char * value ) ;

/**
 * copy a char value into a scilab string in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnChar(char value ) ;

/**
 * copy a single double in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnDouble(double value ) ;

/**
 * copy a single int in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnInt(int value ) ;

/**
 * copy a double array into a row vector in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnRowVector(const double values[], int nbValues ) ;

/**
* copy a int array into a row vector (of double) in the scilab stack
* in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occurred.
*/
void* sciReturnRowVectorFromInt(const int values[], int nbValues ) ;

/**
 * copy an int array into an int row vector in the scilab stack
 * in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnRowIntVector(const int values[], int nbValues ) ;

/**
 * copy a char * array into an string row vector in the scilab stack
 * in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnRowStringVector(char * values[], int nbValues ) ;

/**
 * copy a handle in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnHandle(long handle ) ;

/**
 * copy an handle array into a row vector in the scilab stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnRowHandleVector(const long handles[], int nbValues ) ;

/**
* copy an handle array into a col vector in the scilab stack in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occurred.
*/
void* sciReturnColHandleVector(const long handles[], int nbValues ) ;

/**
 * copy a matrix stored has a double array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrix(double values[], int nbRow, int nbCol ) ;

/**
 * copy a matrix stored has a char * array
 * into a Scilab string matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnStringMatrix(char * values[], int nbRow, int nbCol ) ;

/**
 * return user data in the scilab console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnUserData(const int * userData, int userDataSize ) ;

/**
 * copy a matrix stored has a double hypermatrix
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnHypermatOfDouble(int dims[], int ndims, double values[]);

/**
 * copy a matrix stored has a int8 hypermatrix
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnHypermatOfInteger8(int dims[], int ndims, char values[]);

/**
 * copy a matrix stored has a uint8 hypermatrix
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnHypermatOfUnsignedInteger8(int dims[], int ndims, unsigned char values[]);

/**
 * copy a matrix stored has a int8 array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrixOfInteger8(char values[], int nbRow, int nbCol);

/**
 * copy a matrix stored has a uint8 array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrixOfUnsignedInteger8(unsigned char values[], int nbRow, int nbCol);

/**
 * copy a matrix stored has a int16 array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrixOfInteger16(short values[], int nbRow, int nbCol);

/**
 * copy a matrix stored has a uint16 array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrixOfUnsignedInteger16(unsigned short values[], int nbRow, int nbCol);

/**
 * copy a matrix stored has a int32 array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrixOfInteger32(int values[], int nbRow, int nbCol);

/**
 * copy a matrix stored has a uint32 array
 * into a Scilab matrix in the stack in order to see it the console.
 * @return 0 if the function was executed correctly, -1 if an error occurred.
 */
void* sciReturnMatrixOfUnsignedInteger32(unsigned int values[], int nbRow, int nbCol);
#ifdef __cplusplus
}
#endif
#endif /* _RETURN_PROPERTY_H_ */
