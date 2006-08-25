/*------------------------------------------------------------------------*/
/* file: sci_xrects.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for xrects routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XRECTS_H_
#define _INT_XRECTS_H_

/**
* interface function for the xrects routine.
*
* @param[in] fname     name of the routine (ie xrects)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xrects( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XRECTS_H_ */
