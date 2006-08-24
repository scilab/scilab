/*------------------------------------------------------------------------*/
/* file: sci_xgetech.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xgetech routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XGETECH_H_
#define _INT_XGETECH_H_

/**
* interface function for the xlfont routine :
* [wrect,frect,logflag,arect]=xgetech()
*
* @param[in] fname     name of the routine (ie xlfont)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xgetech( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XGETECH_H_ */
