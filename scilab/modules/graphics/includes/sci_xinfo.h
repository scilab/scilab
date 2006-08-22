/*------------------------------------------------------------------------*/
/* file: sci_xinfo.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xinfo routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XINFO_H_
#define _INT_XINFO_H_

/**
* interface function for the xinfo routine :
* xinfo(string)
*
* @param[in] fname     name of the routine (ie xinfo)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xinfo( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XINFO_H_ */
