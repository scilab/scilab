/*------------------------------------------------------------------------*/
/* file: sci_xsetech.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xsetech routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSETECH_H_
#define _INT_XSETECH_H_

/**
* interface function for the xsetech routine :
* xsetech(wrect=[...],frect=[..],logflag="..", arect=[...])
* or xsetech(wrect,[frect,logflag])
* or xsetech()
*
* @param[in] fname     name of the routine (ie xsetech)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xsetech( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSETECH_H_ */
