/*------------------------------------------------------------------------*/
/* file: sci_xgraduate.h                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xgraduate routine                                 */
/*------------------------------------------------------------------------*/

#ifndef _INT_XGRADUATE_H_
#define _INT_XGRADUATE_H_

/**
* interface function for the xgraduate routine :
* [xi,xa,np1,np2,kMinr,kMaxr,ar]=xgraduate(xmi,xma) 
*
* @param[in] fname     name of the routine (ie xgraduate)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xgraduate( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XGRADUATE_H_ */
