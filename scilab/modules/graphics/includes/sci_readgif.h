/*------------------------------------------------------------------------*/
/* file: sci_readgif.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for sci_readgif routine                               */
/*------------------------------------------------------------------------*/

#ifndef _INT_READGIF_H_
#define _INT_READGIF_H_
/**
* interface function for the readgif routine.
*
* @param[in] fname     name of the routine (ie readgif)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_readgif( char * fname, unsigned long fname_len ) ;

#endif /*_INT_READGIF_H_*/
/*-----------------------------------------------------------------------------------*/ 
