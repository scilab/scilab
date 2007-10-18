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
  getCameraJavaMapper()->moveViewingArea(m_aViewingTranslation[0], m_aViewingTranslation[1],
                                         m_aViewingScale[0], m_aViewingScale[1]) ;
  getCameraJavaMapper()->moveAxesBox(m_aAxesScale[0], m_aAxesScale[1], m_aAxesScale[2],
                                     m_aAxesTranslation[0], m_aAxesTranslation[1], m_aAxesTranslation[2] ) ;

  getCameraJavaMapper()->rotateAxesBox(m_aBoxCenter[0], m_aBoxCenter[1], m_aBoxCenter[2],
                                       m_dAlpha, m_dTheta, m_dFitWindowRatio ) ;
  endDrawing();
}
/*-----------------------------------------------------------------------------------*/
CameraJavaMapper * CameraJoGL::getCameraJavaMapper(void)
{
  return dynamic_cast<CameraJavaMapper *>(getJavaMapper());
}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::getPixelCoordinates(const double userCoord[3], double pixCoord[2])
{
  int * pixCoords = getCameraJavaMapper()->getScreenCoordinates(userCoord[0], userCoord[1], userCoord[2]);
  pixCoord[0] = pixCoord[0];
  pixCoord[1] = pixCoord[1];
  delete[] pixCoords;
}
/*-----------------------------------------------------------------------------------*/
}
