/*------------------------------------------------------------------------*/
/* file: sci_newaxes.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for newaxes routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_NEWAXES_H_
#define _INT_NEWAXES_H_

/**
* interface function for the newaxes routine.
*
* @param[in] fname     name of the routine (ie newaxes)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_newaxes( char * fname, unsigned long fname_len ) ;

#endif /* _INT_NEWAXES_H_ */
