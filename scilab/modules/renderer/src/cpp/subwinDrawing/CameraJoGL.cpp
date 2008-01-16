/*------------------------------------------------------------------------*/
/* file: CameraJoGL.cpp                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : JoGL class to draw a camera                                     */
/*------------------------------------------------------------------------*/

#include "CameraJoGL.h"
#include "DrawableSubwin.h"

extern "C"
{
#include "math_graphics.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
CameraJoGL::CameraJoGL( Camera * camera )
  : CameraBridge(), DrawableObjectJoGL(camera)
{
  setToIdentity(m_aProjMatrix2D);
  setToIdentity(m_aProjMatrix3D);
  setToIdentity(m_aUnprojMatrix2D);
  setToIdentity(m_aUnprojMatrix3D);

  m_aViewPort[0] = 0.0;
  m_aViewPort[1] = 0.0;
  m_aViewPort[2] = 0.0;
  m_aViewPort[3] = 0.0;
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::renderPosition( void )
{
  getCameraJavaMapper()->setViewingArea(m_aViewingTranslation[0], m_aViewingTranslation[1],
                                         m_aViewingScale[0], m_aViewingScale[1]) ;
  getCameraJavaMapper()->setNormalizationParameters(m_aAxesNormalizationScale[0], m_aAxesNormalizationScale[1], m_aAxesNormalizationScale[2],
                                                    m_aAxesTranslation[0], m_aAxesTranslation[1], m_aAxesTranslation[2] ) ;


  getCameraJavaMapper()->setAxesRotationParameters(m_aBoxCenter[0], m_aBoxCenter[1], m_aBoxCenter[2],
                                                   m_dAlpha, m_dTheta);

  getCameraJavaMapper()->setFittingScale(m_aAxesFittingScale[0], m_aAxesFittingScale[1], m_aAxesFittingScale[2]);

  getCameraJavaMapper()->setAxesReverse(m_aAxesReverse[0], m_aAxesReverse[1], m_aAxesReverse[2]);

  getCameraJavaMapper()->placeCamera();

  // save camera viewing settings
  getCameraJavaMapper()->getProjectionMatrix(m_aProjMatrix3D);
  getCameraJavaMapper()->getUnprojectMatrix(m_aUnprojMatrix3D);
  getCameraJavaMapper()->get2dViewProjectionMatrix(m_aProjMatrix2D);
  getCameraJavaMapper()->get2dViewUnprojectMatrix(m_aUnprojMatrix2D);
  getCameraJavaMapper()->getViewPort(m_aViewPort);

}
/*--------------------------------------------------------------------------*/
void CameraJoGL::show(void)
{
  DrawableObjectJoGL::show();
  // save camera viewing settings
  getCameraJavaMapper()->getProjectionMatrix(m_aProjMatrix3D);
  getCameraJavaMapper()->getUnprojectMatrix(m_aUnprojMatrix3D);
  getCameraJavaMapper()->get2dViewProjectionMatrix(m_aProjMatrix2D);
  getCameraJavaMapper()->get2dViewUnprojectMatrix(m_aUnprojMatrix2D);
  getCameraJavaMapper()->getViewPort(m_aViewPort);
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::replaceCamera( void )
{
  getCameraJavaMapper()->replaceCamera();
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::project(const double projMatrix[4][4], const double viewPort[4],
                         const double sceneCoordinates[3], double pixCoordinates[3])
{

  mat4DMult(projMatrix, sceneCoordinates, pixCoordinates);
  pixCoordinates[0] = viewPort[0] + viewPort[2] * (pixCoordinates[0] + 1.0) / 2.0;
  pixCoordinates[1] = viewPort[1] + viewPort[3] * (pixCoordinates[1] + 1.0) / 2.0;
  pixCoordinates[2] = (viewPort[2] + 1.0) / 2.0;
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::unProject(double unProjMatrix[4][4], double viewPort[4],
                           const double pixCoordinates[3], double sceneCoordinates[3])
{
  double viewPix[3];
  viewPix[0] = 2.0 * (pixCoordinates[0] - viewPort[0]) / viewPort[2] - 1.0;
  viewPix[1] = 2.0 * (pixCoordinates[1] - viewPort[1]) / viewPort[3] - 1.0;
  viewPix[2] = 2.0 * pixCoordinates[2] - 1.0;

  mat4DMult(unProjMatrix, viewPix, sceneCoordinates);
}
/*--------------------------------------------------------------------------*/
CameraJavaMapper * CameraJoGL::getCameraJavaMapper(void)
{
  return dynamic_cast<CameraJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  double finalUserCoord[3];
  // convert user coordinates to log scale if needed
  m_pDrawer->pointScale(userCoord[0], userCoord[1], userCoord[2],
                        &(finalUserCoord[0]), &(finalUserCoord[1]), (&finalUserCoord[2]));
  
  // project point on the screen
  double pixCoordsD[3];
  project(m_aProjMatrix3D, m_aViewPort, finalUserCoord, pixCoordsD);
  
  // get the two pixels coordinates
  pixCoord[0] = (int) pixCoordsD[0];
  pixCoord[1] = (int) pixCoordsD[1];
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  double finalUserCoord[3];
  // convert user coordinates to log scale if needed
  m_pDrawer->pointScale(userCoord[0], userCoord[1], userCoord[2],
                        &(finalUserCoord[0]), &(finalUserCoord[1]), (&finalUserCoord[2]));

  // project point on the screen
  double pixCoordsD[3];
  project(m_aProjMatrix2D, m_aViewPort, finalUserCoord, pixCoordsD);

  // get the two pixels coordinates
  pixCoord[0] = (int) pixCoordsD[0];
  pixCoord[1] = (int) pixCoordsD[1];
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::get2dViewCoordinates(const int pixCoords[2], double userCoord2D[2])
{
  double pixCoordsD[3] = {pixCoords[0], pixCoords[1], 0.0};
  double sceneCoords[3];
  unProject(m_aUnprojMatrix2D, m_aViewPort, pixCoordsD, sceneCoords);

  // convert user coordinates to log scale if needed
  m_pDrawer->inversePointScale(sceneCoords[0], sceneCoords[1], sceneCoords[2],
                               &(userCoord2D[0]), &(userCoord2D[1]), NULL);
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getViewingArea(int * xPos, int * yPos, int * width, int * height)
{
  *xPos = m_aViewPort[0] + m_aViewingTranslation[0] * m_aViewPort[2];
  *yPos = m_aViewPort[1] + m_aViewingTranslation[1] * m_aViewPort[3];

  *width = m_aViewPort[2] * m_aViewingScale[0];
  *height = m_aViewPort[3] * m_aViewingScale[1];
}
/*--------------------------------------------------------------------------*/
}
