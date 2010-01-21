/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Startegy drawing the marks at the end of segment
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "SegsMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJoGL::SegsMarkDrawerJoGL( DrawableSegs * segs )
  : DrawSegsStrategy(segs), DrawableObjectJoGL(segs)
{
  setJavaMapper(new SegsMarkDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJoGL::~SegsMarkDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJoGL::drawSegs(const double xStarts[], const double xEnds[],
                                  const double yStarts[], const double yEnds[],
                                  const double zStarts[], const double zEnds[],
                                  const int colors[], int nbSegment)
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();

  initializeDrawing();
  getMarkDrawerJavaMapper()->setMarkParameters(sciGetGraphicContext(pSegs)->markbackground,
                                               sciGetGraphicContext(pSegs)->markforeground,
                                               sciGetMarkSizeUnit(pSegs),
                                               sciGetMarkSize(pSegs),
                                               sciGetMarkStyle(pSegs));
  try
  {
    getMarkDrawerJavaMapper()->drawSegs(xStarts, xEnds,
                                        yStarts, yEnds,
                                        zStarts, zEnds,
                                        nbSegment);
  }
  catch (const std::exception & e)
  {
    sciprint(const_cast<char*>(_("%s: No more memory.\n")),"SegsMarkDrawerJoGL::drawSegs");
  }

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJoGL::showSegs( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJoGL::redrawSegs( void )
{
  initializeDrawing();
  getMarkDrawerJavaMapper()->drawSegs();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJavaMapper * SegsMarkDrawerJoGL::getMarkDrawerJavaMapper(void)
{
  return dynamic_cast<SegsMarkDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
