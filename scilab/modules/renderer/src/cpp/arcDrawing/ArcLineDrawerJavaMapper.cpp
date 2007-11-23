/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerJavaMapper.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by ArcLineDrawer           */
/*------------------------------------------------------------------------*/

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
void ArcLineDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void ArcLineDrawerJavaMapper::setLineParameters(int lineColor, float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(lineColor, thickness, lineStyle);
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

}
