/*------------------------------------------------------------------------*/
/* file: sci_xs2ps.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2ps routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2PS_H_
#define _INT_XS2PS_H_

/**
 * interface function for the xs2ps routine :
 *
 * @param[in] fname     name of the routine (ie xs2ps)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2ps( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2PS_H_ */
