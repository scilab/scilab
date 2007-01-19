/*------------------------------------------------------------------------*/
/* file: sci_show_window.h                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for show_window routine                               */
/*------------------------------------------------------------------------*/

#ifndef _INT_SHOW_WINDOW_H_
#define _INT_SHOW_WINDOW_H_

/**
* interface function for the show_window routine.
*
* @param[in] fname     name of the routine (ie show_window)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_show_window( char * fname, unsigned long fname_len ) ;

#endif /* _INT_SHOW_WINDOW_H_ */
