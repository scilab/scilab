/*------------------------------------------------------------------------*/
/* file: sci_xarcs.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xarcs routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XARCS_H_
#define _INT_XARCS_H_

/**
* interface function for the xarc routine.
*
* @param[in] fname     name of the routine (ie xarcs)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xarcs( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XARCS_H_ */
