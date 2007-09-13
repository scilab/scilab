/*------------------------------------------------------------------------*/
/* file: PolylineInterpColorDrawerJoGL.cpp                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy filling a polygon with interpolated colors             */
/*------------------------------------------------------------------------*/


#include "PolylineInterpColorDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJoGL::PolylineInterpColorDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineInterpColorDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJoGL::~PolylineInterpColorDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJoGL::drawPolyline( void )
{
  initializeDrawing() ;

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;
  int    * colors     = NULL;

  nbVertices = m_pDrawed->getDrawnVerticesLength();
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];
  colors  = new int[nbVertices];

  if (xCoords == NULL || yCoords == NULL || zCoords == NULL)
  {
    sciprint("Unable to render polyline, memory full.\n");
    if (xCoords != NULL) { delete[] xCoords; }
    if (yCoords != NULL) { delete[] yCoords; }
    if (zCoords != NULL) { delete[] zCoords; }
    if (colors != NULL ) { delete[] colors;  }
    endDrawing();
    return;
  }

  m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);
  m_pDrawed->getDrawnVerticesColor(colors);

  // display the rectangle
  getInterpColorDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, colors, nbVertices);

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  delete[] colors;
  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJoGL::showPolyline( void )
{
  show();
}
/*------------------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJavaMapper * PolylineInterpColorDrawerJoGL::getInterpColorDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineInterpColorDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
