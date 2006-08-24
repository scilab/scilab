/*------------------------------------------------------------------------*/
/* file: sci_xstringl.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstringl routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSTRINGL_H_
#define _INT_XSTRINGL_H_

/**
* interface function for the xstringl routine :
* rect=xstringl(x,y,str [,font_id [,font_size]])
*
* @param[in] fname     name of the routine (ie xstringl)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xstringl( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSTRINGL_H_ */
