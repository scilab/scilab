/*------------------------------------------------------------------------*/
/* file: sci_swap_handles.h                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for swap_handles routine                              */
/*------------------------------------------------------------------------*/

#ifndef _INT_SWAP_HANDLES_H_
#define _INT_SWAP_HANDLES_H_

/**
* interface function for the swap_handles routine.
*
* This method is called to swap two handles position in the hierarchy.
* The input should be two single handle which will be swaped.
*
* @param[in] fname     name of the routine (ie swap_handles)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_swap_handles( char * fname, unsigned long fname_len ) ;

#endif /* _INT_SWAP_HANDLES_H_ */
