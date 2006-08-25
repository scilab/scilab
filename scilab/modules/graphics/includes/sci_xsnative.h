/*------------------------------------------------------------------------*/
/* file: sci_xsnative.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for clc routine                                       */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSNATIVE_H_
#define _INT_XSNATIVE_H_

/**
* interface function for the xsnative routine.
*
* @param[in] fname     name of the routine (ie xsnative)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xsnative( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSNATIVE_H_ */
