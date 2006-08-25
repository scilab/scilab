/*------------------------------------------------------------------------*/
/* file: sci_relocate_handle.h                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for relocate_handle routine                           */
/*------------------------------------------------------------------------*/

#ifndef _INT_RELOCATE_HANDLE_H_
#define _INT_RELOCATE_HANDLE_H_

/**
* interface function for the relocate_handle routine.
*
* Method called for relocating handles. The input should contains a vector of handles
* which will be relocated and a single handle which is the new parent.
*
* @param[in] fname     name of the routine (ie relocate_handle)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_relocate_handle( char * fname, unsigned long fname_len ) ;

#endif /* _INT_RELOCATE_HANDLE_H_ */
