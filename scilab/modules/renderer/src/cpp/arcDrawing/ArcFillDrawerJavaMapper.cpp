/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by ArcLineDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ArcFillDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
ArcFillDrawerJavaMapper::ArcFillDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_arcDrawing::ArcFillDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
ArcFillDrawerJavaMapper::~ArcFillDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::setBackColor(int color)
{
  m_pJavaObject->setBackColor(color);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::setUseNurbs(bool useNurbs)
{
  m_pJavaObject->setUseNurbs(useNurbs);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::drawArc( double centerX       , double centerY       , double centerZ       ,
                                       double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
                                       double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
                                       double startAngle    , double endAngle )
{
  m_pJavaObject->drawArc(centerX, centerY, centerZ,
                         semiMinorAxisX, semiMinorAxisY, semiMinorAxisZ,
                         semiMajorAxisX, semiMajorAxisY, semiMajorAxisZ,
                         startAngle    , endAngle);
}
/*---------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::redrawArc(void)
{
  m_pJavaObject->redrawArc();
}
/*---------------------------------------------------------------------------------*/

}
