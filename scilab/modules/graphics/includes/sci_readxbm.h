/*------------------------------------------------------------------------*/
/* file: sci_readxbm.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for sci_readxbm routine                               */
/*------------------------------------------------------------------------*/

#ifndef _INT_READXBM_H_
#define _INT_READXBM_H_
/**
* interface function for the readxbm routine.
*
* @param[in] fname     name of the routine (ie readxbm)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_readxbm( char * fname, unsigned long fname_len ) ;

#endif /*_INT_READXBM_H_*/
/*-----------------------------------------------------------------------------------*/ 
