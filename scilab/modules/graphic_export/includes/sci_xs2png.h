/*------------------------------------------------------------------------*/
/* file: sci_xs2png.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2png routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2PNG_H_
#define _INT_XS2PNG_H_

/**
 * interface function for the xs2png routine :
 *
 * @param[in] fname     name of the routine (ie xs2png)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2png( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2PNG_H_ */
