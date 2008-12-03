/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by SegsLineDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SegsLineDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
SegsLineDrawerJavaMapper::SegsLineDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_segsDrawing::SegsLineDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJavaMapper::~SegsLineDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::destroy(int parentFigureIndex)
{
 m_pJavaObject->destroy(parentFigureIndex); 
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::setLineParameters(float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::drawSegs(const double startXCoords[], const double endXCoords[],
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

}
