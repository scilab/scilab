/*------------------------------------------------------------------------*/
/* file: sci_drawlater.h                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for drawlater routine                                 */
/*------------------------------------------------------------------------*/

#ifndef _INT_DRAWLATER_H_
#define _INT_DRAWLATER_H_

/**
* interface function for the drawlater routine.
*
* @param[in] fname     name of the routine (ie drawlater)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_drawlater( char * fname, unsigned long fname_len ) ;

#endif /* _INT_DRAWLATER_H_ */
