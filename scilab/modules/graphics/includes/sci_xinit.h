/*------------------------------------------------------------------------*/
/* file: sci_xinit.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xinit routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XINIT_H_
#define _INT_XINIT_H_

/**
* interface function for the xinit routine :
* xinit([driver-name])
*
* @param[in] fname     name of the routine (ie xinit)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xinit( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XINIT_H_ */
