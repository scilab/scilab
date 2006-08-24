/*------------------------------------------------------------------------*/
/* file: sci_xpoly.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xpoly routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XPOLY_H_
#define _INT_XPOLY_H_

/**
* interface function for the xpoly routine :
* xpoly(xv,yv,dtype,[close])
*
* @param[in] fname     name of the routine (ie xpoly)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xpoly( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XPOLY_H_ */
