/*------------------------------------------------------------------------*/
/* file: PolylineLineDrawerJoGL.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the line of a polyline object                  */
/*------------------------------------------------------------------------*/

#include "PolylineLineDrawerJoGL.hxx"

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
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];

  if (xCoords == NULL || yCoords == NULL || zCoords == NULL)
  {
    sciprint(_("Unable to render polyline, memory full.\n"));
    if(xCoords != NULL) { delete[] xCoords; }
    if(yCoords != NULL) { delete[] yCoords; }
    if(zCoords != NULL) { delete[] zCoords; }
    endDrawing();
    return;
  }

  m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);

  // display the rectangle
  getLineDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);

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
