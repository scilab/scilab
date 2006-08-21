/*------------------------------------------------------------------------*/
/* file: sci_draw.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for draw routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_DRAW_H_
#define _INT_DRAW_H_

/**
* interface function for the draw routine :
*
* @param[in] fname      name of the routine (ie draw)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_draw( char * fname, unsigned long fname_len ) ;

#endif /* _INT_DRAW_H_ */
