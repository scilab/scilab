/*------------------------------------------------------------------------*/
/* file: sci_drawnow.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for drawnow routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_DRAWNOW_H_
#define _INT_DRAWNOW_H_

/**
* interface function for the drawnow routine.
*
* @param[in] fname     name of the routine (ie drawnow)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_drawnow( char * fname, unsigned long fname_len ) ;

#endif /* _INT_DRAWNOW_H_ */
