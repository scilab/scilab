/*------------------------------------------------------------------------*/
/* file: sci_get.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : interface for sci_get routine                                   */
/*------------------------------------------------------------------------*/
#ifndef _INT_GET_H_
#define _INT_GET_H_

/**
* interface function for the get routine.
*
* @param[in] fname     name of the routine (ie get)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_get(char *fname,unsigned long fname_len);

/*-----------------------------------------------------------------------------------*/
/* copy the string value into the scilab stack for output */
int sciReturnString( const char * value ) ;

/* copy an int in the scilab stack for output */
int sciReturnInt( int value ) ;

/* copy a row vector in the scilab stack for output */
int sciReturnRowVector( double values[], int nbValues ) ;

/*-----------------------------------------------------------------------------------*/

#endif /*_INT_GET_H_*/
/*-----------------------------------------------------------------------------------*/
