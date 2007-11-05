/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing bar plots                                      */
/*------------------------------------------------------------------------*/


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

/*------------------------------------------------------------------------------------------*/
PolylineBarDrawerJoGL::PolylineBarDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineBarDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
PolylineBarDrawerJoGL::~PolylineBarDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void PolylineBarDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;


  double bar_width;
  if (sciGetPolylineStyle(pObj) == 3)
  {
    // bar plot mode
    bar_width = 0.0;
  }
  else
  {
    bar_width = pPOLYLINE_FEATURE(pObj)->bar_width;
  }

  // set the line parameters
  getBarDrawerJavaMapper()->setBarParameters(sciGetGraphicContext(pObj)->backgroundcolor,
                                             sciGetGraphicContext(pObj)->foregroundcolor,
                                            (float)sciGetLineWidth(pObj),
                                             sciGetLineStyle(pObj),
                                             bar_width);

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;
  double * height     = NULL;

  // special case here, we must use a specila decomosition for bars.
  BarDecomposition decomposer(m_pDrawed);

  nbVertices = decomposer.getDrawnVerticesLength();
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];
  height  = new double[nbVertices];

  if (xCoords == NULL || yCoords == NULL || zCoords == NULL || height == NULL)
  {
    sciprint(_("Unable to render polyline, memory full.\n"));
    if (xCoords != NULL) { delete[] xCoords; }
    if (yCoords != NULL) { delete[] yCoords; }
    if (zCoords != NULL) { delete[] zCoords; }
    if (height  != NULL) { delete[] height;  }
    endDrawing();
    return;
  }

  decomposer.getDrawnVertices(xCoords, yCoords, zCoords);
  decomposer.getBarHeight(height);

  // display the rectangle
  getBarDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, height, nbVertices);

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  delete[] height;
  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void PolylineBarDrawerJoGL::showPolyline( void )
{
  show();
}
/*------------------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper * PolylineBarDrawerJoGL::getBarDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineBarDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
