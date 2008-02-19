/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of Java method used by PolylineBarDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "PolylineBarDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper::PolylineBarDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineBarDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper::~PolylineBarDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::setBarParameters(int background, int foreground, float thickness,
                                                   int lineStyle)
{
  m_pJavaObject->setBarParameters(background, foreground, thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::drawPolyline(const double left[],
                                               const double right[],
                                               const double bottom[],
                                               const double top[],
                                               const double zCoord[],
                                               int nbCoords)
{
  m_pJavaObject->drawPolyline((double *)left, nbCoords,
                              (double *)right, nbCoords,
                              (double *)bottom, nbCoords,
                              (double *)top, nbCoords,
                              (double *)zCoord, nbCoords); 
}
/*---------------------------------------------------------------------------------*/
}
