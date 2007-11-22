/*------------------------------------------------------------------------*/
/* file: IsoViewCameraJavaMapper.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by Camera for isoview      */
/*        renderering                                                     */
/*------------------------------------------------------------------------*/

#include "IsoViewCameraJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*-----------------------------------------------------------------------------------*/
IsoViewCameraJavaMapper::IsoViewCameraJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::IsoViewCameraGL(getScilabJavaVM());
}
/*-----------------------------------------------------------------------------------*/
IsoViewCameraJavaMapper::~IsoViewCameraJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setViewingArea(double transX, double transY, double scaleX, double scaleY)
{
  m_pJavaObject->setViewingArea(transX, transY, scaleX, scaleY);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setNormalizationParameters(double scaleX, double scaleY, double scaleZ,
                                                  double transX, double transY, double transZ)
{
  m_pJavaObject->setNormalizationParameters(scaleX, scaleY, scaleZ,
                                            transX, transY, transZ);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setAxesRotationParameters(double centerX, double centerY, double centerZ,
                                                  double alpha, double theta)
{
  m_pJavaObject->setAxesRotationParameters(centerX, centerY, centerZ, alpha, theta);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setFittingScale(double scaleX, double scaleY, double scaleZ)
{
  m_pJavaObject->setFittingScale(scaleX, scaleY, scaleZ);
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::placeCamera(void)
{
  m_pJavaObject->placeCamera();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::replaceCamera( void )
{
  m_pJavaObject->replaceCamera();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::revertXAxis(void)
{
  m_pJavaObject->revertXAxis();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::revertYAxis(void)
{
  m_pJavaObject->revertYAxis();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::revertZAxis(void)
{
  m_pJavaObject->revertZAxis();
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ,
                                            int pixCoord[2])
{
  long * javaCoords = m_pJavaObject->get2dViewPixelCoordinates(userCoordX, userCoordY, userCoordZ);
  pixCoord[0] = javaCoords[0];
  pixCoord[1] = javaCoords[1];

  delete[] javaCoords;
}
/*-----------------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ,
                                           int pixCoord[2])
{
  long * javaCoords = m_pJavaObject->get2dViewPixelCoordinates(userCoordX, userCoordY, userCoordZ);
  pixCoord[0] = javaCoords[0];
  pixCoord[1] = javaCoords[1];

  delete[] javaCoords;
}
/*-----------------------------------------------------------------------------------*/

}
