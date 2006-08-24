/*------------------------------------------------------------------------*/
/* file: sci_xrect.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for xrect routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XRECT_H_
#define _INT_XRECT_H_

/**
* interface function for the xrect routine :
* xrect(x,y,w,h)
*
* @param[in] fname     name of the routine (ie xrect)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xrect( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XRECT_H_ */
