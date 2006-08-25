/*------------------------------------------------------------------------*/
/* file: sci_copy.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for copy routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_COPY_H_
#define _INT_COPY_H_

/**
* interface function for the copy routine.
*
* @param[in] fname     name of the routine (ie copy)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_copy( char * fname, unsigned long fname_len ) ;

#endif /* _INT_COPY_H_ */
