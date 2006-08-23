/*------------------------------------------------------------------------*/
/* file: sci_xsort.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xsort routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSORT_H_
#define _INT_XSORT_H_

/**
* interface function for the xsort routine :
* [s,v]= gsort(x,[str1,str2])
*       str1 = 'g','r','c','lc','lr',
*       str2 = 'i' | 'd'
*
* @param[in] fname     name of the routine (ie xsort)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xsort( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSORT_H_ */
