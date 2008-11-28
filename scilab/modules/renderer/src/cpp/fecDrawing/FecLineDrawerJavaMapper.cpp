/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by FecLineDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FecLineDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
FecLineDrawerJavaMapper::FecLineDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_fecDrawing::FecLineDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
FecLineDrawerJavaMapper::~FecLineDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex); 
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::setLineParameters(int lineColor, float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(lineColor, thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJavaMapper::drawFec(const double xCoords[], const double yCoords[],
                                      const double values[], int nbNodes,
                                      const int firstPoints[], const int secondPoints[],
                                      const int thirdPoints[], int nbTriangles)
{
  m_pJavaObject->drawFec((double *)xCoords, nbNodes,
                         (double *)yCoords, nbNodes,
                         (double *)values, nbNodes,
                         (int*)firstPoints, nbTriangles,
                         (int*)secondPoints, nbTriangles,
                         (int*)thirdPoints, nbTriangles);
}
/*---------------------------------------------------------------------------------*/

}
