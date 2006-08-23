/*------------------------------------------------------------------------*/
/* file: sci_xdel.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XDEL_H_
#define _INT_XDEL_H_

/**
* interface function for the xdel routine :
* xdel([win-ids])
*
* @param[in] fname     name of the routine (ie xdel)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xdel( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XDEL_H_ */
