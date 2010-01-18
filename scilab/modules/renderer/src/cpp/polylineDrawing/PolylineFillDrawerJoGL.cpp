/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy filling the inside of a polygon curve
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <exception>

#include "PolylineFillDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineFillDrawerJoGL::PolylineFillDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineFillDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineFillDrawerJoGL::~PolylineFillDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  if ( sciGetPolylineStyle(pObj) == 5)
  {
    // strange mode
    getFillDrawerJavaMapper()->setBackColor(sciGetGraphicContext(pObj)->foregroundcolor) ;
  }
  else
  {
    // normal mode
    getFillDrawerJavaMapper()->setBackColor(sciGetGraphicContext(pObj)->backgroundcolor) ;
  }
  

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;

  nbVertices = m_pDrawed->getDrawnVerticesLength();
  try
  {
    xCoords = new double[nbVertices];
    yCoords = new double[nbVertices];
    zCoords = new double[nbVertices];
  }
  catch (const std::exception& e)
  {
    // allocation failed
	  sciprint(const_cast<char*>(_("%s: No more memory.\n")),"PolylineFillDrawerJoGL::drawPolyline");
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
    getFillDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);
  }
  catch (const std::exception & e)
  {
	  sciprint(const_cast<char*>(_("%s: No more memory.\n")),"PolylineFillDrawerJoGL::drawPolyline");
  }

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
PolylineFillDrawerJavaMapper * PolylineFillDrawerJoGL::getFillDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineFillDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
