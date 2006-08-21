/*------------------------------------------------------------------------*/
/* file: sci_xarrows.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xarrows routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XARROWS_H_
#define _INT_XARROWS_H_

/**
* interface function for the xarrows routine :
* xarrows(nx,ny,[arsize,style])
*
* @param[in] fname     name of the routine (ie xarrows)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xarrows( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XFARCS_H_ */
