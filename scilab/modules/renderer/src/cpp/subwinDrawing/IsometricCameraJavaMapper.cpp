/*------------------------------------------------------------------------*/
/* file: IsometricCameraJavaMapper.cpp                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by Camera                  */
/*------------------------------------------------------------------------*/

#include "IsometricCameraJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*--------------------------------------------------------------------------*/
IsometricCameraJavaMapper::IsometricCameraJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::IsometricCameraGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
IsometricCameraJavaMapper::~IsometricCameraJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::setViewingArea(double transX, double transY, double scaleX, double scaleY)
{
  m_pJavaObject->setViewingArea(transX, transY, scaleX, scaleY);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::setNormalizationParameters(double scaleX, double scaleY, double scaleZ,
                                                           double transX, double transY, double transZ)
{
  m_pJavaObject->setNormalizationParameters(scaleX, scaleY, scaleZ,
    transX, transY, transZ);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::setAxesRotationParameters(double centerX, double centerY, double centerZ,
                                                          double alpha, double theta)
{
  m_pJavaObject->setAxesRotationParameters(centerX, centerY, centerZ, alpha, theta);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::setFittingScale(double scaleX, double scaleY, double scaleZ)
{
  m_pJavaObject->setFittingScale(scaleX, scaleY, scaleZ);
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::placeCamera(void)
{
  m_pJavaObject->placeCamera();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::replaceCamera( void )
{
  m_pJavaObject->replaceCamera();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::revertXAxis(void)
{
  m_pJavaObject->revertXAxis();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::revertYAxis(void)
{
  m_pJavaObject->revertYAxis();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::revertZAxis(void)
{
  m_pJavaObject->revertZAxis();
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ,
                                                    int pixCoord[2])
{
  long * javaCoords = m_pJavaObject->get2dViewPixelCoordinates(userCoordX, userCoordY, userCoordZ);
  pixCoord[0] = javaCoords[0];
  pixCoord[1] = javaCoords[1];

  delete[] javaCoords;
}
/*--------------------------------------------------------------------------*/
void IsometricCameraJavaMapper::get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ,
  int pixCoord[2])
{
  long * javaCoords = m_pJavaObject->get2dViewPixelCoordinates(userCoordX, userCoordY, userCoordZ);
  pixCoord[0] = javaCoords[0];
  pixCoord[1] = javaCoords[1];

  delete[] javaCoords;
}
/*--------------------------------------------------------------------------*/

}
