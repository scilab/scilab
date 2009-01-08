/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by SurfaceMarkDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "SurfaceMarkDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SurfaceMarkDrawerJavaMapper::SurfaceMarkDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_surfaceDrawing::SurfaceMarkDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SurfaceMarkDrawerJavaMapper::~SurfaceMarkDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::setMarkParameters(int background, int foreground, int markSizeUnit,
                                                    int markSize, int markStyleIndex)
{
  m_pJavaObject->setMarkParameters(background, foreground, markSizeUnit, markSize, markStyleIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::setSurfaceType(int typeOfSurface)
{
  m_pJavaObject->setSurfaceType(typeOfSurface);
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::drawSurface(double xCoords[], int sizeX,
                                              double yCoords[], int sizeY,
                                              double zCoords[], int sizeZ,
                                              int nbFacets)
{
  m_pJavaObject->drawSurface(xCoords, sizeX, yCoords, sizeY, zCoords, sizeZ, nbFacets);
}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJavaMapper::drawSurface(void)
{
  m_pJavaObject->drawSurface();
}
/*---------------------------------------------------------------------------------*/
}
