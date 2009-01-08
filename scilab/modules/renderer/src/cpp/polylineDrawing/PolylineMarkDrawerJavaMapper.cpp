/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by PolylineLineDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "PolylineMarkDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJavaMapper::PolylineMarkDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineMarkDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJavaMapper::~PolylineMarkDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::setMarkParameters(int background, int foreground, int markSizeUnit, int markSize, int markStyleIndex)
{
  m_pJavaObject->setMarkParameters(background, foreground, markSizeUnit, markSize, markStyleIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::drawPolyline(const double xCoords[], const double yCoords[], const double zCoords[], int nbCoords)
{
  m_pJavaObject->drawPolyline((double *)xCoords, nbCoords,
                              (double *)yCoords, nbCoords,
                              (double *)zCoords, nbCoords);
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJavaMapper::drawPolyline(void)
{
  m_pJavaObject->drawPolyline();
}
/*---------------------------------------------------------------------------------*/
}
