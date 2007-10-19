/*------------------------------------------------------------------------*/
/* file: PolylineArrowDrawerJoGL.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing arrow heads on a polyline object               */
/*------------------------------------------------------------------------*/


#include "PolylineArrowDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
PolylineArrowDrawerJoGL::PolylineArrowDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineArrowDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
PolylineArrowDrawerJoGL::~PolylineArrowDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
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
  getArrowDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJoGL::showPolyline( void )
{
  show();
}
/*------------------------------------------------------------------------------------------*/
PolylineArrowDrawerJavaMapper * PolylineArrowDrawerJoGL::getArrowDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineArrowDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
