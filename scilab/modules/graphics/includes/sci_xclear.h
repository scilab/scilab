/*------------------------------------------------------------------------*/
/* file: sci_xclear.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclear routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XCLEAR_H_
#define _INT_XCLEAR_H_

/**
* interface function for the xclear routine :
* xclear([window-ids])
*
* @param[in] fname     name of the routine (ie xclear)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xclear( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XCLEAR_H_ */
