/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: Vertices.h                                                       */
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
