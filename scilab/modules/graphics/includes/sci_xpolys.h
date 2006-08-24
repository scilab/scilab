/*------------------------------------------------------------------------*/
/* file: sci_xpolys.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xpolys routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XPOLYS_H_
#define _INT_XPOLYS_H_

/**
* interface function for the xpolys routine :
* xpolys(xpols,ypols,[draw])
*
* @param[in] fname     name of the routine (ie xpolys)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xpolys( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XPOLYS_H_ */
