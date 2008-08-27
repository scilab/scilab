/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : JoGL class to draw a camera 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CameraJoGL.h"
#include "DrawableSubwin.h"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
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
CameraJoGL::~CameraJoGL(void)
{
  
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

  double bounds[6];
  sciGetRealDataBounds(m_pDrawer->getDrawedObject(), bounds);
  getCameraJavaMapper()->setAxesBounds(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);

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
  
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::redraw(void)
{
  getCameraJavaMapper()->redrawCamera();
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::replaceCamera( void )
{
  getCameraJavaMapper()->replaceCamera();
  // save camera viewing settings
  /*getCameraJavaMapper()->getProjectionMatrix(m_aProjMatrix3D);
  getCameraJavaMapper()->getUnprojectMatrix(m_aUnprojMatrix3D);
  getCameraJavaMapper()->get2dViewProjectionMatrix(m_aProjMatrix2D);
  getCameraJavaMapper()->get2dViewUnprojectMatrix(m_aUnprojMatrix2D);
  getCameraJavaMapper()->getViewPort(m_aViewPort);*/
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::project(const double projMatrix[4][4], const double viewPort[4],
                         const double sceneCoordinates[3], double pixCoordinates[3])
{

  mat4DMult(projMatrix, sceneCoordinates, pixCoordinates);
  pixCoordinates[0] = viewPort[0] + viewPort[2] * (pixCoordinates[0] + 1.0) / 2.0;
  pixCoordinates[1] = viewPort[1] + viewPort[3] * (pixCoordinates[1] + 1.0) / 2.0;
  pixCoordinates[2] = (pixCoordinates[2] + 1.0) / 2.0;
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
  // project point on the screen
  double pixCoords3D[3];
  getPixelCoordinates(userCoord, pixCoords3D);
  
  // get the two pixels coordinates
  pixCoord[0] = (int) pixCoords3D[0];
  
  pixCoord[1] = (int) pixCoords3D[1];
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getPixelCoordinates(const double userCoords[3], double pixCoords[3])
{
  double finalUserCoord[3];
  // convert user coordinates to log scale if needed
  m_pDrawer->pointScale(userCoords[0], userCoords[1], userCoords[2],
                        &(finalUserCoord[0]), &(finalUserCoord[1]), (&finalUserCoord[2]));

  getPixelCoordinatesRaw(finalUserCoord, pixCoords);
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getPixelCoordinatesRaw(const double userCoords[3], double pixCoords[3])
{
  // project point on the screen
  project(m_aProjMatrix3D, m_aViewPort, userCoords, pixCoords);

  // in OpenGL, pixel coordinates are taken from the bottom left point
  // of the window, however, in Scilab it is from the top left
  // so we have to invert Y coordinate
  pixCoords[1] = m_aViewPort[3] - pixCoords[1];
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  double pixCoord3D[3];
  get2dViewPixelCoordinates(userCoord, pixCoord3D);

  // get it in pixel coordinates
  pixCoord[0] = (int) pixCoord3D[0];
  pixCoord[1] = (int) pixCoord3D[1];
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::get2dViewPixelCoordinates(const double userCoord[3], double pixCoord[3])
{
  double finalUserCoord[3];
  // convert user coordinates to log scale if needed
  m_pDrawer->pointScale(userCoord[0], userCoord[1], userCoord[2],
                        &(finalUserCoord[0]), &(finalUserCoord[1]), (&finalUserCoord[2]));

  // project point on the screen
  project(m_aProjMatrix2D, m_aViewPort, finalUserCoord, pixCoord);

  // get the two pixels coordinates
  //pixCoord[0] = pixCoordsD[0];
  // in OpenGL, pixel coordinates are taken from the bottom left point
  // of the window, however, in Scilab it is from the top left
  // so we have to invert Y coordinate
  pixCoord[1] = (m_aViewPort[3] - pixCoord[1]);
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getSceneCoordinates(const double pixCoords[3], double userCoords[3])
{
  
  getSceneCoordinatesRaw(pixCoords, userCoords);

  // convert user coordinates to log scale if needed
  m_pDrawer->inversePointScale(userCoords[0], userCoords[1], userCoords[2],
                              &(userCoords[0]), &(userCoords[1]), &(userCoords[2]));
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getSceneCoordinatesRaw(const double pixCoords[3], double userCoords[3])
{
  // in OpenGL, pixel coordinates are taken from the bottom left point
  // of the window, however, in Scilab it is from the top left
  // so we have to invert Y coordinate
  double pixCoords3D[3] = {pixCoords[0], m_aViewPort[3] - pixCoords[1], pixCoords[2]};
  unProject(m_aUnprojMatrix3D, m_aViewPort, pixCoords3D, userCoords);
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::get2dViewCoordinates(const int pixCoords[2], double userCoord2D[2])
{
  // in OpenGL, pixel coordinates are taken from the bottom left point
  // of the window, however, in Scilab it is from the top left
  // so we have to invert Y coordinate
  double pixCoordsD[3] = {pixCoords[0], m_aViewPort[3] - pixCoords[1], 0.0};
  double sceneCoords[3];
  unProject(m_aUnprojMatrix2D, m_aViewPort, pixCoordsD, sceneCoords);

  // convert user coordinates to log scale if needed
  m_pDrawer->inversePointScale(sceneCoords[0], sceneCoords[1], sceneCoords[2],
                               &(userCoord2D[0]), &(userCoord2D[1]), NULL);
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getViewingArea(int * xPos, int * yPos, int * width, int * height)
{
  *xPos = (int)(m_aViewPort[0] + m_aViewingTranslation[0] * m_aViewPort[2]);
  //*yPos =(int)(m_aViewPort[1] + m_aViewingTranslation[1] * m_aViewPort[3]);
  
  

  *width = (int)(m_aViewPort[2] * m_aViewingScale[0]);
  *height = (int)(m_aViewPort[3] * m_aViewingScale[1]);

  // invert on y axis
  *yPos = (int) (m_aViewPort[1]  + m_aViewPort[3] * (1.0 - m_aViewingTranslation[1])) - *height;
}
/*--------------------------------------------------------------------------*/
void CameraJoGL::getViewport(int viewport[4])
{
  for (int i = 0; i < 4; i++)
  {
    viewport[i] = (int)m_aViewPort[i];
  }
}
/*--------------------------------------------------------------------------*/
}
