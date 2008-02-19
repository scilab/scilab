/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Startegy drawing the head of segment arrows
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SegsArrowDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJoGL::SegsArrowDrawerJoGL( DrawableSegs * segs )
  : DrawSegsStrategy(segs), DrawableObjectJoGL(segs)
{
  setJavaMapper(new SegsArrowDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJoGL::~SegsArrowDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJoGL::drawSegs(const double xStarts[], const double xEnds[],
                                   const double yStarts[], const double yEnds[],
                                   const double zStarts[], const double zEnds[],
                                   const int colors[], int nbSegment)
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();

  initializeDrawing();
  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(pSegs), bounds);
  getArrowDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1],
                                            bounds[2], bounds[3],
                                            bounds[4], bounds[5]);

  getArrowDrawerJavaMapper()->setArrowSize(sciGetLineWidth(pSegs) * sciGetArrowSize(pSegs));
  getArrowDrawerJavaMapper()->drawSegs(xStarts, xEnds,
                                       yStarts, yEnds,
                                       zStarts, zEnds,
                                       colors, nbSegment);
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJoGL::showSegs( void )
{
  initializeDrawing();

  // update bounds, they may have changed
  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(m_pDrawed->getDrawedObject()), bounds);
  getArrowDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1],
                                            bounds[2], bounds[3],
                                            bounds[4], bounds[5]);
  show();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJavaMapper * SegsArrowDrawerJoGL::getArrowDrawerJavaMapper(void)
{
  return dynamic_cast<SegsArrowDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
