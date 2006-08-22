/*------------------------------------------------------------------------*/
/* file: sci_xstringb.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstringb routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSTRINGB_H_
#define _INT_XSTRINGB_H_

/**
* interface function for the xstringb routine :
*
* @param[in] fname     name of the routine (ie xstringb)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xstringb( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSTRINGB_H_ */
