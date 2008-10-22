/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy for the facets of a fec object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>

#include "FecFacetDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJoGL::FecFacetDrawerJoGL(DrawableFec * fec)
  : DrawFecStrategy(), DrawableObjectJoGL(fec)
{
  setJavaMapper(new FecFacetDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJoGL::~FecFacetDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJoGL::drawFec(const double xCoords[], const double yCoords[],
                                 const double values[], int nbNodes,
  const int firstPoints[], const int secondPoints[],
  const int thirdPoints[], int nbTriangles)
{
  sciPointObj * pFec = getDrawer()->getDrawedObject();
  sciFec * ppFec = pFEC_FEATURE(pFec);
  initializeDrawing();

  getFacetDrawerJavaMapper()->setFacetParameters(ppFec->zminmax[0], ppFec->zminmax[1],
                                                 ppFec->colminmax[0], ppFec->colminmax[1],
                                                 ppFec->colout[0], ppFec->colout[1]);
  
  try
  {
    getFacetDrawerJavaMapper()->drawFec(xCoords, yCoords, values, nbNodes,
                                        firstPoints, secondPoints, thirdPoints, nbTriangles);
  }
  catch (const std::exception & e)
  {
     sciprint(_("%s: No more memory.\n"), "FecFacetDrawerJoGL::drawFec");
  }

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJoGL::showFec(void)
{
  show();
}
/*---------------------------------------------------------------------------------*/
DrawableFec * FecFacetDrawerJoGL::getFecDrawer(void)
{
  return dynamic_cast<DrawableFec *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJavaMapper * FecFacetDrawerJoGL::getFacetDrawerJavaMapper(void)
{
  return dynamic_cast<FecFacetDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
