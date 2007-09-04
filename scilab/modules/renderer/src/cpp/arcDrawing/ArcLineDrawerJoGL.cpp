/*------------------------------------------------------------------------*/
/* file: RectangleLineDrawerJoGL.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the outline of a rectangle                     */
/*------------------------------------------------------------------------*/

#include "ArcLineDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ArcLineDrawerJoGL::ArcLineDrawerJoGL( DrawableArc * arc )
  : DrawArcStrategy(arc), DrawableObjectJoGL(arc)
{

}
/*------------------------------------------------------------------------------------------*/
ArcLineDrawerJoGL::~ArcLineDrawerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void ArcLineDrawerJoGL::drawArc( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pObj)->foregroundcolor,
                                               (float)sciGetLineWidth(pObj),
                                               sciGetLineStyle(pObj));

  // get the data of the arc
  double center[3];
  double semiMinorAxis[3];
  double semiMajorAxis[3];
  double startAngle;
  double endAngle;

  m_pDrawed->getArcRepresentation(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle ) ;

  // display the rectangle
  getLineDrawerJavaMapper()->drawArc(center[0]       , center[1]       , center[2]       ,
                                     semiMinorAxis[0], semiMinorAxis[1], semiMinorAxis[2],
                                     semiMajorAxis[0], semiMajorAxis[1], semiMajorAxis[2],
                                     startAngle      , endAngle);
  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void ArcLineDrawerJoGL::showArc( void )
{
  show();
}
/*------------------------------------------------------------------------------------------*/
ArcLineDrawerJavaMapper * ArcLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<ArcLineDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}

