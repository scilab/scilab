/*------------------------------------------------------------------------*/
/* file: sci_uiwait.h                                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : interface for sci_uiwait routine                                */
/*------------------------------------------------------------------------*/

#ifndef __INT_UIWAIT_H__
#define __INT_UIWAIT_H__

/**
* interface function for the uiwait routine.
*
* @param[in] fname     name of the routine (ie uiwait)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_uiwait( char * fname, unsigned long fname_len ) ;

#endif /* !__INT_UIWAIT_H__ */
