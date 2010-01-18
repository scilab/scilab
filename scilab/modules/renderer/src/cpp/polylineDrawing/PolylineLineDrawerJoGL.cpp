/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing the line of a polyline object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "PolylineLineDrawerJoGL.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineLineDrawerJoGL::PolylineLineDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineLineDrawerJoGL::~PolylineLineDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineLineDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pObj)->foregroundcolor,
                                               (float)sciGetLineWidth(pObj),
                                               sciGetLineStyle(pObj));

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;

  nbVertices = m_pDrawed->getDrawnVerticesLength();
  try {
    xCoords = new double[nbVertices];
    yCoords = new double[nbVertices];
    zCoords = new double[nbVertices];
  }
  catch (const std::exception& e)
  {
    // allocation failed
	  sciprint(const_cast<char*>(_("%s: No more memory.\n")), "PolylineLineDrawerJoGL::drawPolyline");
    if(xCoords != NULL) { delete[] xCoords; }
    if(yCoords != NULL) { delete[] yCoords; }
    if(zCoords != NULL) { delete[] zCoords; }
    endDrawing();
    return;
  }

  m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);

  // display the polyline
  try {
    getLineDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);
  }
  catch (const std::exception& e)
  {
	  sciprint(const_cast<char*>(_("%s: No more memory.\n")),"PolylineLineDrawerJoGL::drawPolyline");
  }

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineLineDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
PolylineLineDrawerJavaMapper * PolylineLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
