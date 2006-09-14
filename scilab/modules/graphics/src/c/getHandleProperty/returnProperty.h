/*------------------------------------------------------------------------*/
/* file: returnProperty.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#ifndef _RETURN_PROPERTY_H_
#define _RETURN_PROPERTY_H_

/**
* copy the string value in the scilab stack in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occured.
*/
int sciReturnString( const char * value ) ;

/**
* copy a single int in the scilab stack in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occured.
*/
int sciReturnInt( int value ) ;

/**
* copy a double array into a row vector in the scilab stack in order to see it the console.
* @return 0 if the function was executed correctly, -1 if an error occured.
*/
int sciReturnRowVector( const double values[], int nbValues ) ;

#endif /* _RETURN_PROPERTY_H_ */