/*------------------------------------------------------------------------*/
/* file: sci_xload.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xload routine                                 */
/*------------------------------------------------------------------------*/

#ifndef _INT_XLOAD_H_
#define _INT_XLOAD_H_

/**
* interface function for the xload routine :
* xload('fname' [, wid]
*
* @param[in] fname     name of the routine (ie xload)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xload( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XLOAD_H_ */
