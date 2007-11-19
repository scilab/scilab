/*------------------------------------------------------------------------*/
/* file: CameraJoGL.cpp                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : JoGL class to draw a camera                                     */
/*------------------------------------------------------------------------*/

#include "CameraJoGL.h"
#include "DrawableSubwin.h"

namespace sciGraphics
{

/*-----------------------------------------------------------------------------------*/
CameraJoGL::CameraJoGL( DrawableSubwin * subwin )
  : CameraBridge(), DrawableObjectJoGL(subwin)
{

}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::renderPosition( void )
{
  initializeDrawing();
  getCameraJavaMapper()->setViewingArea(m_aViewingTranslation[0], m_aViewingTranslation[1],
                                         m_aViewingScale[0], m_aViewingScale[1]) ;
  getCameraJavaMapper()->setNormalizationParameters(m_aAxesScale[0], m_aAxesScale[1], m_aAxesScale[2],
                                                    m_aAxesTranslation[0], m_aAxesTranslation[1], m_aAxesTranslation[2] ) ;


  getCameraJavaMapper()->setAxesRotationParameters(m_aBoxCenter[0], m_aBoxCenter[1], m_aBoxCenter[2],
                                                   m_dAlpha, m_dTheta);

  double minScale = Min(m_aAxesScale[0], Min(m_aAxesScale[1], m_aAxesScale[2]));
  getCameraJavaMapper()->setFittingScale(minScale, minScale, minScale);
  //                                     m_aAxesScale[0], m_aAxesScale[1], m_aAxesScale[2]) ;
  endDrawing();
}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::replaceCamera( void )
{
  getCameraJavaMapper()->replaceCamera();
}
/*-----------------------------------------------------------------------------------*/
CameraJavaMapper * CameraJoGL::getCameraJavaMapper(void)
{
  return dynamic_cast<CameraJavaMapper *>(getJavaMapper());
}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::getPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraJavaMapper()->getPixelCoordinates(userCoord[0], userCoord[1], userCoord[2], pixCoord);
}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraJavaMapper()->get2dViewPixelCoordinates(userCoord[0], userCoord[1], userCoord[2], pixCoord);
}
/*-----------------------------------------------------------------------------------*/
}
