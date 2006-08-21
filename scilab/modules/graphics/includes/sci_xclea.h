/*------------------------------------------------------------------------*/
/* file: sci_xclea.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclea routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XCLEA_H_
#define _INT_XCLEA_H_

/**
* interface function for the xclea routine :
* xclea(x,y,w,h) etendu a xclea([x,y,w,h])
*
* @param[in] fname     name of the routine (ie xclea)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xclea( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XCHANGE_H_ */
