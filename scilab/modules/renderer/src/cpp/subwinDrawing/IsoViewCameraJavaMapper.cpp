/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by Camera for isoview
 * renderering
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "IsoViewCameraJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*--------------------------------------------------------------------------*/
IsoViewCameraJavaMapper::IsoViewCameraJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::IsoViewCameraGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
IsoViewCameraJavaMapper::~IsoViewCameraJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setViewingArea(double transX, double transY, double scaleX, double scaleY)
{
  m_pJavaObject->setViewingArea(transX, transY, scaleX, scaleY);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setNormalizationParameters(double scaleX, double scaleY, double scaleZ,
                                                  double transX, double transY, double transZ)
{
  m_pJavaObject->setNormalizationParameters(scaleX, scaleY, scaleZ,
                                            transX, transY, transZ);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setAxesRotationParameters(double centerX, double centerY, double centerZ,
                                                  double alpha, double theta)
{
  m_pJavaObject->setAxesRotationParameters(centerX, centerY, centerZ, alpha, theta);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setFittingScale(double scaleX, double scaleY, double scaleZ)
{
  m_pJavaObject->setFittingScale(scaleX, scaleY, scaleZ);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::placeCamera(void)
{
  m_pJavaObject->placeCamera();
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::redrawCamera(void)
{
  m_pJavaObject->redrawCamera();
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::replaceCamera( void )
{
  m_pJavaObject->replaceCamera();
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setAxesReverse(bool xAxisRevert, bool yAxisRevert, bool zAxisRevert)
{
  m_pJavaObject->setAxesReverse(xAxisRevert, yAxisRevert, zAxisRevert);
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::getPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ,
                                            int pixCoord[2])
{
  int * javaCoords = m_pJavaObject->get2dViewPixelCoordinates(userCoordX, userCoordY, userCoordZ);
  pixCoord[0] = javaCoords[0];
  pixCoord[1] = javaCoords[1];

  delete[] javaCoords;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::get2dViewPixelCoordinates(double userCoordX, double userCoordY, double userCoordZ,
                                           int pixCoord[2])
{
  int * javaCoords = m_pJavaObject->get2dViewPixelCoordinates(userCoordX, userCoordY, userCoordZ);
  pixCoord[0] = javaCoords[0];
  pixCoord[1] = javaCoords[1];

  delete[] javaCoords;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::getProjectionMatrix(double mat[4][4])
{
  double * matArray = m_pJavaObject->getProjectionMatrix();
  convertMatrixFormat(matArray, mat);
  delete[] matArray;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::getUnprojectMatrix(double mat[4][4])
{
  double * matArray = m_pJavaObject->getUnprojectMatrix();
  convertMatrixFormat(matArray, mat);
  delete[] matArray;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::get2dViewProjectionMatrix(double mat[4][4])
{
  double * matArray = m_pJavaObject->get2dViewProjectionMatrix();
  convertMatrixFormat(matArray, mat);
  delete[] matArray;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::get2dViewUnprojectMatrix(double mat[4][4])
{
  double * matArray = m_pJavaObject->get2dViewUnprojectMatrix();
  convertMatrixFormat(matArray, mat);
  delete[] matArray;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::getViewPort(double viewPort[4])
{
  double * javaViewPort = m_pJavaObject->getViewPort();
  viewPort[0] = javaViewPort[0];
  viewPort[1] = javaViewPort[1];
  viewPort[2] = javaViewPort[2];
  viewPort[3] = javaViewPort[3];
  delete[] javaViewPort;
}
/*--------------------------------------------------------------------------*/
void IsoViewCameraJavaMapper::setAxesBounds(double xMin, double xMax,
				            double yMin, double yMax,
				            double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
}
