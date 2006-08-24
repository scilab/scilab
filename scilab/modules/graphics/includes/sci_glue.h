/*------------------------------------------------------------------------*/
/* file: sci_glue.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for glue routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_GLUE_H_
#define _INT_GLUE_H_

/**
* interface function for the glue routine.
*
* @param[in] fname     name of the routine (ie glue)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_glue( char * fname, unsigned long fname_len ) ;

#endif /* _INT_GLUE_H_ */
