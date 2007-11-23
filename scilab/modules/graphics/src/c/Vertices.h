/*------------------------------------------------------------------------*/
/* file: Vertices.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions to store vertices for the zoom               */
/*------------------------------------------------------------------------*/

#ifndef _VERTICES_H_
#define _VERTICES_H_

#include "ObjectStructure.h"

/*----------------------------------------------------------------------------------*/
int  getVertexIndex( void ) ;
void setVertexIndex( int ind ) ;
/*----------------------------------------------------------------------------------*/
int SetMinMaxVertices( Vertices * vertices_list, double * xmin, double * ymin, double * zmin,double * xmax, double * ymax, double * zmax ) ;
int GetVerticesAt( Vertices * vertices_list, int * xm, int * ym, double * x, double * y, double * z ) ;
int RemoveNext( Vertices * pCurrent ) ;
int FreeVertices( sciPointObj * psubwin ) ;
/*----------------------------------------------------------------------------------*/
int Store3DPixelValues( sciPointObj * pobj, int xm, int ym, double x, double y, double z ) ;
/*----------------------------------------------------------------------------------*/

#endif /* _VERTICES_H_ */
