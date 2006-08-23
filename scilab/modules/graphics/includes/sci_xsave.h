/*------------------------------------------------------------------------*/
/* file: sci_xsave.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xsave routine                                 */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSAVE_H_
#define _INT_XSAVE_H_

/**
* interface function for the xgetmouse routine :
* xsave('fname' [, wid])
*
* @param[in] fname     name of the routine (ie xgetmouse)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xsave( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSAVE_H_ */
