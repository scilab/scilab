/*------------------------------------------------------------------------*/
/* file: sci_xs2gif.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2gif routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2GIF_H_
#define _INT_XS2GIF_H_

/**
* interface function for the xs2gif routine.
*
* @param[in] fname     name of the routine (ie xs2gif)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xs2gif( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2GIF_H_ */
