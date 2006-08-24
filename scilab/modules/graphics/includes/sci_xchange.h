/*------------------------------------------------------------------------*/
/* file: sci_xchange.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xchange routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_XCHANGE_H_
#define _INT_XCHANGE_H_

/**
* interface function for the xchange routine :
* [x1,y1,rect]=xchange(x,y,dir)
*
* @param[in] fname     name of the routine (ie xchange)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xchange( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XCHANGE_H_ */
