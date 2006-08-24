/*------------------------------------------------------------------------*/
/* file: sci_xset.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSET_H_
#define _INT_XSET_H_

/**
* interface function for the xset routine :
*
* @param[in] fname     name of the routine (ie xset)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xset( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSET_H_ */
