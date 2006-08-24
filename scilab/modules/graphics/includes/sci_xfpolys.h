/*------------------------------------------------------------------------*/
/* file: sci_xfpolys.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xfpolys routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XFPOLYS_H_
#define _INT_XFPOLYS_H_

/**
* interface function for the newaxes routine :
* xfpolys(xpols,ypols,[fill])
*
* @param[in] fname     name of the routine (ie xfpolys)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xfpolys( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XFPOLYS_H_ */
