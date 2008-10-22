/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing arrow heads on a polyline object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "PolylineArrowDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineArrowDrawerJoGL::PolylineArrowDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineArrowDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineArrowDrawerJoGL::~PolylineArrowDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineArrowDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getArrowDrawerJavaMapper()->setArrowParameters(sciGetGraphicContext(pObj)->foregroundcolor,
                                                 sciGetArrowSize(pObj) * sciGetLineWidth(pObj));

  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(pObj), bounds);
  getArrowDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1],
                                            bounds[2], bounds[3],
                                            bounds[4], bounds[5]);

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
    sciprint(_("%s: No more memory.\n"),"PolylineArrowDrawerJoGL::drawPolyline");
    if(xCoords != NULL) { delete[] xCoords; }
    if(yCoords != NULL) { delete[] yCoords; }
    if(zCoords != NULL) { delete[] zCoords; }
    endDrawing();
    return;
   }

  m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);

  // display the rectangle
  try
  {
    getArrowDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);
  }
  catch (const std::exception& e)
  {
    sciprint(_("%s: No more memory.\n"),"PolylineArrowDrawerJoGL::drawPolyline");
  }



  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineArrowDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
void PolylineArrowDrawerJoGL::redrawPolyline( void )
{
  initializeDrawing();
  // axes may have changed
  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(m_pDrawed->getDrawedObject()), bounds);
  getArrowDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1],
                                            bounds[2], bounds[3],
                                            bounds[4], bounds[5]);
  getArrowDrawerJavaMapper()->drawPolyline();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
PolylineArrowDrawerJavaMapper * PolylineArrowDrawerJoGL::getArrowDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineArrowDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
