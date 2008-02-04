/*------------------------------------------------------------------------*/
/* file: sci_xs2pdf.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2pdf routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2PDF_H_
#define _INT_XS2PDF_H_

/**
 * interface function for the xs2pdf routine :
 *
 * @param[in] fname     name of the routine (ie xs2pdf)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2pdf( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2PDF_H_ */
