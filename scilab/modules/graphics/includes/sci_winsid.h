/*------------------------------------------------------------------------*/
/* file: sci_winsid.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for winsid routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_WINSID_H_
#define _INT_WINSID_H_

/**
* interface function for the winsid routine :
* x=winsid()
*
* @param[in] fname     name of the routine (ie winsid)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_winsid( char * fname, unsigned long fname_len ) ;

#endif /* _INT_WINSID_H_ */
