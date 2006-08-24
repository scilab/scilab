/*------------------------------------------------------------------------*/
/* file: sci_unglue.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for unglue routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_UNGLUE_H_
#define _INT_UNGLUE_H_

/**
* interface function for the unglue routine.
*
* @param[in] fname     name of the routine (ie unglue)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_unglue( char * fname, unsigned long fname_len ) ;

#endif /* _INT_UNGLUE_H_ */
