/*------------------------------------------------------------------------*/
/* file: sci_set_posfig_dim.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for set_posfig_dim routine                            */
/*------------------------------------------------------------------------*/

#ifndef _INT_SET_POSFIG_DIM_H_
#define _INT_SET_POSFIG_DIM_H_

/**
* interface function for the set_posfig_dim routine :
* set_posfig_dim(width,height)
*
* @param[in] fname     name of the routine (ie set_posfig_dim)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_set_posfig_dim( char * fname, unsigned long fname_len ) ;

#endif /* _INT_SET_POSFIG_DIM_H_ */
