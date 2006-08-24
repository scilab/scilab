/*------------------------------------------------------------------------*/
/* file: sci_xname.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xname routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XNAME_H_
#define _INT_XNAME_H_

/**
* interface function for the xname routine :
* xname('name') 
*
* @param[in] fname     name of the routine (ie xname)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xname( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XNAME_H_ */
