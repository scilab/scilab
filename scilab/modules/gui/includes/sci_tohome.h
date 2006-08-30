/*------------------------------------------------------------------------*/
/* file: sci_tohome.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for tohome routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_TOHOME_H_
#define _INT_TOHOME_H_

/**
* interface function for the tohome routine.
*
* @param[in] fname     name of the routine (ie tohome)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_tohome( char * fname, unsigned long fname_len ) ;

#endif /* _INT_TOHOME_H_ */
