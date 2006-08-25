/*------------------------------------------------------------------------*/
/* file: sci_xg2ps.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xg2ps routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XG2PS_H_
#define _INT_XG2PS_H_

/**
* interface function for the xg2ps routine :
*
* @param[in] fname     name of the routine (ie xg2ps)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xg2ps( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XG2PS_H_ */
