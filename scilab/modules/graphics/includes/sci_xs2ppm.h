/*------------------------------------------------------------------------*/
/* file: sci_xs2ppm.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2ppm routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2PPM_H_
#define _INT_XS2PPM_H_

/**
* interface function for the xs2ppm routine.
*
* @param[in] fname     name of the routine (ie xs2ppm)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xs2ppm( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2PPM_H_ */
