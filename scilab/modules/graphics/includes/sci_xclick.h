/*------------------------------------------------------------------------*/
/* file: sci_xclick.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclick routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XCLICK_H_
#define _INT_XCLICK_H_

/**
* interface function for the xclear routine :
* [c_i,c_x,c_y,c_w,c_m]=xclick([flag])
*
* @param[in] fname     name of the routine (ie xclick)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xclick( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XCLICK_H_ */
