/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing the outline of a rectangle 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ArcLineDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ArcLineDrawerJoGL::ArcLineDrawerJoGL( DrawableArc * arc )
  : DrawArcStrategy(arc), DrawableObjectJoGL(arc)
{

}
/*---------------------------------------------------------------------------------*/
ArcLineDrawerJoGL::~ArcLineDrawerJoGL(void)
{
  
}
/*---------------------------------------------------------------------------------*/
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

  getLineDrawerJavaMapper()->setUseNurbs(sciGetUseNurbs(pObj) == TRUE);

  // display the rectangle
  getLineDrawerJavaMapper()->drawArc(center[0]       , center[1]       , center[2]       ,
                                     semiMinorAxis[0], semiMinorAxis[1], semiMinorAxis[2],
                                     semiMajorAxis[0], semiMajorAxis[1], semiMajorAxis[2],
                                     startAngle      , endAngle);
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJoGL::redrawArc(void)
{
  getLineDrawerJavaMapper()->redrawArc();
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJoGL::showArc( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
ArcLineDrawerJavaMapper * ArcLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<ArcLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}

