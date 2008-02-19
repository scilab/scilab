/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by SurfaceFacetDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SurfaceLineDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SurfaceLineDrawerJavaMapper::SurfaceLineDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_surfaceDrawing::SurfaceLineDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SurfaceLineDrawerJavaMapper::~SurfaceLineDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::setLineParameters(int lineColor, float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(lineColor, thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::setSurfaceType(int typeOfSurface)
{
  m_pJavaObject->setSurfaceType(typeOfSurface);
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJavaMapper::drawSurface(double xCoords[], int sizeX,
                                              double yCoords[], int sizeY,
                                              double zCoords[], int sizeZ,
                                              int nbFacets)
{
  m_pJavaObject->drawSurface(xCoords, sizeX, yCoords, sizeY, zCoords, sizeZ, nbFacets);
}
/*---------------------------------------------------------------------------------*/
}
