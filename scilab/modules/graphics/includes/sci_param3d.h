/*------------------------------------------------------------------------*/
/* file: sci_param3d.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for param3d routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_PARAM3D_H_
#define _INT_PARAM3D_H_

/**
* interface function for the param3d routine :
* param3d(x,y,z,[theta,alpha,leg,flag,ebox])
*
* @param[in] fname      name of the routine (ie param3d)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sciparam3d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PARAM3D_H_ */
