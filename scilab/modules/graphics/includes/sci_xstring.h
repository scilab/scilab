/*------------------------------------------------------------------------*/
/* file: sci_xtring.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstring routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSTRING_H_
#define _INT_XSTRING_H_

/**
* interface function for the xset routine :
* xstring(x,y,str,[angle,box])
*
* @param[in] fname     name of the routine (ie xstring)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xstring( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSTRING_H_ */
