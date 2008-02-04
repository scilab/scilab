/*------------------------------------------------------------------------*/
/* file: sci_xs2eps.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2eps routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2EPS_H_
#define _INT_XS2EPS_H_

/**
 * interface function for the xs2eps routine :
 *
 * @param[in] fname     name of the routine (ie xs2eps)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2eps( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2EPS_H_ */
