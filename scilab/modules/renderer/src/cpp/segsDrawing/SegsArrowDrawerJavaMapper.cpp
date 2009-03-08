/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by SegsArrowDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SegsArrowDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJavaMapper::SegsArrowDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_segsDrawing::SegsArrowDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJavaMapper::~SegsArrowDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex); 
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setArrowSize(double size)
{
  m_pJavaObject->setArrowSize(size);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setIsSegs(bool isSegs)
{
  m_pJavaObject->setIsSegs(isSegs);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                              double yMin, double yMax,
                                              double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::drawSegs(const double startXCoords[], const double endXCoords[],
                                         const double startYCoords[], const double endYCoords[],
                                         const double startZCoords[], const double endZCoords[],
                                         const int colors[], int nbSegments)
{
  m_pJavaObject->drawSegs((double *)startXCoords, nbSegments,
                          (double *)endXCoords, nbSegments,
                          (double *)startYCoords, nbSegments,
                          (double *)endYCoords, nbSegments,
                          (double *)startZCoords, nbSegments,
                          (double *)endZCoords, nbSegments,
                          (int *)colors, nbSegments);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::drawSegs(void)
{
  m_pJavaObject->drawSegs();
}
/*---------------------------------------------------------------------------------*/
}
