/*------------------------------------------------------------------------*/
/* file: sci_xtitle.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xtitle routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XTITLE_H_
#define _INT_XTITLE_H_

/**
* interface function for the xtitle routine :
* xtitle(tit,x,y)
*
* @param[in] fname     name of the routine (ie xtitle)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xtitle( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XTITLE_H_ */
