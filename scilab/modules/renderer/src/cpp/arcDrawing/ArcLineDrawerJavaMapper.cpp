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

#include "ArcLineDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
ArcLineDrawerJavaMapper::ArcLineDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_arcDrawing::ArcLineDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
ArcLineDrawerJavaMapper::~ArcLineDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex); 
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::setLineParameters(int lineColor, float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(lineColor, thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::setUseNurbs(bool useNurbs)
{
  m_pJavaObject->setUseNurbs(useNurbs);
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::drawArc( double centerX       , double centerY       , double centerZ       ,
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
void ArcLineDrawerJavaMapper::redrawArc(void)
{
  m_pJavaObject->redrawArc();
}
/*---------------------------------------------------------------------------------*/

}
