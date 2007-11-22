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
CameraJoGL::CameraJoGL( Camera * camera )
  : CameraBridge(), DrawableObjectJoGL(camera)
{

}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::renderPosition( void )
{
  getCameraJavaMapper()->setViewingArea(m_aViewingTranslation[0], m_aViewingTranslation[1],
                                         m_aViewingScale[0], m_aViewingScale[1]) ;
  getCameraJavaMapper()->setNormalizationParameters(m_aAxesNormalizationScale[0], m_aAxesNormalizationScale[1], m_aAxesNormalizationScale[2],
                                                    m_aAxesTranslation[0], m_aAxesTranslation[1], m_aAxesTranslation[2] ) ;


  getCameraJavaMapper()->setAxesRotationParameters(m_aBoxCenter[0], m_aBoxCenter[1], m_aBoxCenter[2],
                                                   m_dAlpha, m_dTheta);

  getCameraJavaMapper()->setFittingScale(m_aAxesFittingScale[0], m_aAxesFittingScale[1], m_aAxesFittingScale[2]);

  getCameraJavaMapper()->placeCamera();
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
