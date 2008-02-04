/*------------------------------------------------------------------------*/
/* file: sci_xs2jpg.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2jpg routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2JPG_H_
#define _INT_XS2JPG_H_

/**
 * interface function for the xs2jpg routine :
 *
 * @param[in] fname     name of the routine (ie xs2jpg)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2jpg( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2JPG_H_ */
