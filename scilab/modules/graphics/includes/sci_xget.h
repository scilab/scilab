/*------------------------------------------------------------------------*/
/* file: sci_xget.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xget routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XGET_H_
#define _INT_XGET_H_

/**
* interface function for the xget routine.
*
* @param[in] fname     name of the routine (ie xget)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xget( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XGET_H_ */
