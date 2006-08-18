/*------------------------------------------------------------------------*/
/* file: sci_param3d1.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for param3d1 routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_PARAM3D1_H_
#define _INT_PARAM3D1_H_

/**
* interface function for the param3d1 routine :
* param3d1(x,y,z,[theta,alpha,leg,flag,ebox])
* param3d1(x,y,list(z,colors),[theta,alpha,leg,flag,ebox])
*
* @param[in] fname      name of the routine (ie param3d1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_param3d1( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PARAM3D1_H_ */