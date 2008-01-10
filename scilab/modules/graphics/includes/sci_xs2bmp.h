/*------------------------------------------------------------------------*/
/* file: sci_xs2bmp.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2bmp routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2BMP_H_
#define _INT_XS2BMP_H_

/**
 * interface function for the xs2bmp routine :
 *
 * @param[in] fname     name of the routine (ie xs2bmp)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2bmp( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2BMP_H_ */
