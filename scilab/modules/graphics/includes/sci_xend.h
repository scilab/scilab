/*------------------------------------------------------------------------*/
/* file: sci_xend.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xend routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XEND_H_
#define _INT_XEND_H_

/**
* interface function for the xend routine.
*
* @param[in] fname     name of the routine (ie xend)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xend( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XEND_H_ */
