/*------------------------------------------------------------------------*/
/* file: sci_xarc.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xarc routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XARC_H_
#define _INT_XARC_H_

/**
* interface function for the xarc routine.
*
* @param[in] fname     name of the routine (ie xarc)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xarc( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XARC_H_ */
