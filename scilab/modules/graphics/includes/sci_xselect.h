/*------------------------------------------------------------------------*/
/* file: sci_xselect.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xselect routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSELECT_H_
#define _INT_XSELECT_H_

/**
* interface function for the xselect routine.
*
* @param[in] fname     name of the routine (ie xselect)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xselect( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSELECT_H_ */
