/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
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
/* file: BasicAlgos.h                                                     */

/* desc : Several basic algorithms which can be used in several places in */
/*        the code.                                                       */
/*------------------------------------------------------------------------*/

#ifndef _BASIC_ALGOS_H_
#define _BASIC_ALGOS_H_
#include "dynlib_graphics.h"
#include "BOOL.h"

/**
 * Find the striclty positive minimum value of an array of double.
 * @param[in] x array of size n.
 * @return The stricly posistive minimum of x if exists, a negative value otherwise
 */
GRAPHICS_IMPEXP double sciFindStPosMin( const double x[], int n ) ;


/**
 * check the strict monotony of a vector
 * @return  1 if the vector is increasing or constant
 *         -1 if decreasing and not constant
 *          0 if not monotonous
 * @param nbElement must be greater than 2.
 */
GRAPHICS_IMPEXP int checkMonotony( const double vector[], int nbElement ) ;

/**
 * @return TRUE if the array contains one element which is not %inf, -%inf or %nan
 *         FALSE is all its elements are not finite
 */
GRAPHICS_IMPEXP BOOL containsOneFiniteElement(const double vector[], int nbElement);

/*--------------------------------------------------------------------------*/
/**
 * copy an array into an other
 */
GRAPHICS_IMPEXP void doubleArrayCopy( double dest[], const double src[], int nbElement ) ;

/**
 * copy an int array into an other
 */
GRAPHICS_IMPEXP void intArrayCopy( int dest[], const int src[], int nbElement ) ;

/**
 * copy a char * array into an other
 */
GRAPHICS_IMPEXP void stringArrayCopy( char * dest[], char * src[], int nbElement ) ;

/**
 * set each element of an array to a certain value
 */
GRAPHICS_IMPEXP void setDoubleArraySingleValue( double dest[], double value, int nbElement ) ;

/**
 * create a new array which is kind of copy of another
 * the elements are copied beginning with the first
 * if the array is longer than the other, then the last values are set to 0.0
 * @param destSize the wanted size for the new array
 * @return  the newly created array or NULL if an error occured
 */
GRAPHICS_IMPEXP double * createNewArrayFromSource( int destSize, const double src[], int srcSize ) ;

/**
 * free an array of strings
 */
GRAPHICS_IMPEXP void destroyStringArray( char * src[], int nbStrings ) ;

/**
 * Free a string allocated by new
 */
GRAPHICS_IMPEXP void destroyCppString(char * string);

/**
 * allocate a copy of a double array
 */
GRAPHICS_IMPEXP double * createDoubleArrayCopy( const double src[], int nbElement ) ;

/**
 * allocate a copy of a int array.
 */
GRAPHICS_IMPEXP int * createIntArrayCopy( const int src[], int nbElement ) ;

/**
 * allocate a copy of a string array
 */
GRAPHICS_IMPEXP char ** createStringArrayCopy( char * src[], int nbElement ) ;

/**
 * allocate a string array and set all its element to NULL
 */
GRAPHICS_IMPEXP char ** createStringArray(int nbElement);
/*--------------------------------------------------------------------------*/

#endif /* _BASIC_ALGOS_H_ */
