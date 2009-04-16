/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Startegy drawing the line part of segment arrows
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "SegsLineDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJoGL::SegsLineDrawerJoGL( DrawableSegs * segs )
: DrawSegsStrategy(segs), DrawableObjectJoGL(segs)
{
  setJavaMapper(new SegsLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJoGL::~SegsLineDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJoGL::drawSegs(const double xStarts[], const double xEnds[],
                                  const double yStarts[], const double yEnds[],
                                  const double zStarts[], const double zEnds[],
                                  const int colors[], int nbSegment)
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();

  initializeDrawing();
  getLineDrawerJavaMapper()->setLineParameters((float)sciGetLineWidth(pSegs), sciGetLineStyle(pSegs));
  try
  {
    getLineDrawerJavaMapper()->drawSegs(xStarts, xEnds,
                                        yStarts, yEnds,
                                        zStarts, zEnds,
                                        colors, nbSegment);
  }
  catch (const std::exception & e)
  {
    sciprint(_("%s: No more memory.\n"),"SegsLineDrawerJoGL::drawSegs");
  }

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJoGL::showSegs( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJavaMapper * SegsLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<SegsLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
