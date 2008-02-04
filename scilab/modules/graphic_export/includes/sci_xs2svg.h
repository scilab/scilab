/*------------------------------------------------------------------------*/
/* file: sci_xs2svg.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2svg routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2SVG_H_
#define _INT_XS2SVG_H_

/**
 * interface function for the xs2svg routine :
 *
 * @param[in] fname     name of the routine (ie xs2svg)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2svg( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2SVG_H_ */
