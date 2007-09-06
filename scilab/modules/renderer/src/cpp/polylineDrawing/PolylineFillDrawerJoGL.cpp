/*------------------------------------------------------------------------*/
/* file: PolylineFillDrawerJoGL.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy filling the inside of a polygon curve                  */
/*------------------------------------------------------------------------*/

#include "PolylineFillDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
PolylineFillDrawerJoGL::PolylineFillDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineFillDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
PolylineFillDrawerJoGL::~PolylineFillDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
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
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];

  if (xCoords == NULL || yCoords == NULL || zCoords == NULL)
  {
    sciprint("Unable to render polyline, memory full.\n");
    if(xCoords != NULL) { delete[] xCoords; }
    if(yCoords != NULL) { delete[] yCoords; }
    if(zCoords != NULL) { delete[] zCoords; }
    endDrawing();
    return;
  }

  m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);

  // display the rectangle
  getFillDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void PolylineFillDrawerJoGL::showPolyline( void )
{
  show();
}
/*------------------------------------------------------------------------------------------*/
PolylineFillDrawerJavaMapper * PolylineFillDrawerJoGL::getFillDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineFillDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
