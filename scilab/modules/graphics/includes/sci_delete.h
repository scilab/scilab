/*------------------------------------------------------------------------*/
/* file: sci_delete.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for delete routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_DELETE_H_
#define _INT_DELETE_H_

/**
* interface function for the delete routine.
*
* @param[in] fname     name of the routine (ie unzoom)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_delete( char * fname, unsigned long fname_len ) ;

#endif /* _INT_DELETE_H_ */
