/*------------------------------------------------------------------------*/
/* file: sci_geom3d.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for geom3d routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_GEOM3D_H_
#define _INT_GEOM3D_H_

/**
* interface function for the geom3d routine :
* [x,y]=geom3d(x1,y1,z1)
*
* @param[in] fname      name of the routine (ie geom3d)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_geom3d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_GEOM3D_H_ */
