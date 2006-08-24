/*------------------------------------------------------------------------*/
/* file: sci_xaxis.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xaxis routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XAXIS_H_
#define _INT_XAXIS_H_

/**
* interface function for the xarrows routine.
*
* @param[in] fname     name of the routine (ie xaxis)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xaxis( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XAXIS_H_ */
