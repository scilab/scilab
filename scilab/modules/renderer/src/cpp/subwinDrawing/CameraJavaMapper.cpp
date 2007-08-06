/*------------------------------------------------------------------------*/
/* file: CameraJavaMapper.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by Camera                  */
/*------------------------------------------------------------------------*/

#include "CameraJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*-----------------------------------------------------------------------------------*/
CameraJavaMapper::CameraJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::CameraGL(getScilabJavaVM());
}
/*-----------------------------------------------------------------------------------*/
CameraJavaMapper::~CameraJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::show(void)
{
  m_pJavaObject->show();
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::moveViewingArea(double transX, double transY, double scaleX, double scaleY)
{
  m_pJavaObject->moveViewingArea(transX, transY, scaleX, scaleY);
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::moveAxesBox(double scaleX, double scaleY, double scaleZ,
                                   double transX, double transY, double transZ)
{
  m_pJavaObject->moveAxesBox(scaleX, scaleY, scaleZ,
                             transX, transY, transZ);
}
/*-----------------------------------------------------------------------------------*/
void CameraJavaMapper::rotateAxesBox(double centerX, double centerY, double centerZ,
                                     double alpha, double theta, double reductionRatio)
{
  m_pJavaObject->rotateAxesBox(centerX, centerY, centerZ,
                               alpha  , theta, reductionRatio);
}
/*-----------------------------------------------------------------------------------*/
}
