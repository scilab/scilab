/*------------------------------------------------------------------------*/
/* file: sci_move.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for move routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_MOVE_H_
#define _INT_MOVE_H_

/**
* interface function for the move routine.
*
* @param[in] fname     name of the routine (ie move)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_move( char * fname, unsigned long fname_len ) ;

#endif /* _INT_MOVE_H_ */
