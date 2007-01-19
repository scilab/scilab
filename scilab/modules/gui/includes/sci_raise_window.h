/*------------------------------------------------------------------------*/
/* file: sci_raise_window.h                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for raise_window routine                              */
/*------------------------------------------------------------------------*/

#ifndef _INT_RAISE_WINDOW_H_
#define _INT_RAISE_WINDOW_H_

/**
* interface function for the raise_window routine.
*
* @param[in] fname     name of the routine (ie clc)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_raise_window( char * fname, unsigned long fname_len ) ;

#endif /* _INT_SHOW_WINDOW_H_ */
