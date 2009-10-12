/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing the marks on a polyline object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "PolylineMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

#include "BarDecomposition.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJoGL::PolylineMarkDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineMarkDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJoGL::~PolylineMarkDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getMarkDrawerJavaMapper()->setMarkParameters(sciGetGraphicContext(pObj)->markbackground,
                                               sciGetGraphicContext(pObj)->markforeground,
                                               sciGetMarkSizeUnit(pObj),
                                               sciGetMarkSize(pObj),
                                               sciGetMarkStyle(pObj));

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;

  // special case for bar plot
  BarDecomposition barPoltDecomposer(m_pDrawed);

  if (sciGetPolylineStyle(pObj) == 3)
  {
    // bar plot also draw marks on y = 0 plane
    nbVertices = barPoltDecomposer.getBarPlotMarkVerticesLength();
  }
  else
  {
    nbVertices = m_pDrawed->getDrawnVerticesLength();
  }

  try
  {
    xCoords = new double[nbVertices];
    yCoords = new double[nbVertices];
    zCoords = new double[nbVertices];
  }
  catch (const std::exception& e)
  {
    // allocation failed
    sciprint(_("%s: No more memory.\n"),"PolylineMarkDrawerJoGL::drawPolyline");
    if(xCoords != NULL) { delete[] xCoords; }
    if(yCoords != NULL) { delete[] yCoords; }
    if(zCoords != NULL) { delete[] zCoords; }
    endDrawing();
    return;
  }

  if (sciGetPolylineStyle(pObj) == 3)
  {
    barPoltDecomposer.getBarPlotMarkVertices(xCoords, yCoords, zCoords);
  }
  else
  {
    m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);
  }

  // display the rectangle
  try
  {
    getMarkDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);
  }
  catch (const std::exception& e)
  {
    sciprint(_("%s: No more memory.\n"),"PolylineMarkDrawerJoGL::drawPolyline");
  }

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJoGL::redrawPolyline( void )
{
  initializeDrawing() ;
  getMarkDrawerJavaMapper()->drawPolyline();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJavaMapper * PolylineMarkDrawerJoGL::getMarkDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineMarkDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
