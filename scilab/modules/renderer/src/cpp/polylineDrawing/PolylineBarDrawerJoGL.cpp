/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing bar plots
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing bar plots                                      */
/*------------------------------------------------------------------------*/

#include <exception>

#include "PolylineBarDrawerJoGL.hxx"
#include "BarDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJoGL::PolylineBarDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineBarDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJoGL::~PolylineBarDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;


  

  // set the line parameters
  getBarDrawerJavaMapper()->setBarParameters(sciGetGraphicContext(pObj)->backgroundcolor,
                                             sciGetGraphicContext(pObj)->foregroundcolor,
                                            (float)sciGetLineWidth(pObj),
                                             sciGetLineStyle(pObj));

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;
  double * bottom     = NULL;
  double * top        = NULL;
  double * left       = NULL;
  double * right      = NULL;

  // special case here, we must use a special decomposition for bars.
  BarDecomposition decomposer(m_pDrawed);

  nbVertices = decomposer.getDrawnVerticesLength();

  try
  {
    xCoords = new double[nbVertices];
    yCoords = new double[nbVertices];
    zCoords = new double[nbVertices];
    bottom  = new double[nbVertices];
    top     = new double[nbVertices];
    left    = new double[nbVertices];
    right   = new double[nbVertices]; 
  }
  catch (const std::exception& e)
  {
    // allocation failed
    sciprint(_("%s: No more memory.\n"),"PolylineBarDrawerJoGL::drawPolyline");
    if (xCoords != NULL) { delete[] xCoords; }
    if (yCoords != NULL) { delete[] yCoords; }
    if (zCoords != NULL) { delete[] zCoords; }
    if (bottom  != NULL) { delete[] bottom;  }
    if (top     != NULL) { delete[] top;     }
    if (left    != NULL) { delete[] left;    }
    if (right   != NULL) { delete[] right;   }
    endDrawing();
    return;
  }

  decomposer.getDrawnVertices(xCoords, yCoords, zCoords);
  decomposer.getBarOrdinates(bottom, top);
  decomposer.getBarAbscissas(left, right);

  // display the rectangle
  try
  {
    getBarDrawerJavaMapper()->drawPolyline(left, right, bottom, top, zCoords, nbVertices);
  }
  catch (const std::exception& e)
  {
    sciprint(_("%s: No more memory.\n"),"PolylineBarDrawerJoGL::drawPolyline");
  }
 
  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  delete[] bottom;
  delete[] top;
  delete[] left;
  delete[] right;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper * PolylineBarDrawerJoGL::getBarDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineBarDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
