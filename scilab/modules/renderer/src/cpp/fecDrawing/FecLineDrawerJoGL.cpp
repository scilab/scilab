/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy for the mesh of a fec object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "FecLineDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
FecLineDrawerJoGL::FecLineDrawerJoGL(DrawableFec * fec)
: DrawFecStrategy(), DrawableObjectJoGL(fec)
{
  setJavaMapper(new FecLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
FecLineDrawerJoGL::~FecLineDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJoGL::drawFec(const double xCoords[], const double yCoords[],
                                const double values[], int nbNodes,
                                const int firstPoints[], const int secondPoints[],
                                const int thirdPoints[], int nbTriangles)
{
  sciPointObj * pFec = getDrawer()->getDrawedObject();
  initializeDrawing();
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pFec)->foregroundcolor,
                                               (float)sciGetLineWidth(pFec),
                                               sciGetLineStyle(pFec));

  try
  {
  getLineDrawerJavaMapper()->drawFec(xCoords, yCoords, values, nbNodes,
                                     firstPoints, secondPoints, thirdPoints, nbTriangles);
  }
  catch (const std::exception & e)
  {
     sciprint(_("%s: No more memory.\n"), "FecLineDrawerJoGL::drawFec");
  }
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJoGL::showFec(void)
{
  show();
}
/*---------------------------------------------------------------------------------*/
DrawableFec * FecLineDrawerJoGL::getFecDrawer(void)
{
  return dynamic_cast<DrawableFec *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
FecLineDrawerJavaMapper * FecLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<FecLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
