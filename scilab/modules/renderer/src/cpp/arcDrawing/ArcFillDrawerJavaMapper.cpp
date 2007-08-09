/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerJavaMapper.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by ArcLineDrawer           */
/*------------------------------------------------------------------------*/

#include "ArcFillDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
ArcFillDrawerJavaMapper::ArcFillDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_arcDrawing::ArcFillDrawerGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
ArcFillDrawerJavaMapper::~ArcFillDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void ArcFillDrawerJavaMapper::setBackColor(int color)
{
  m_pJavaObject->setBackColor(color);
}
/*------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------*/

}
