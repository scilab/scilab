/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Abstract class for text drawing
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: TextContentDrawerJoGL.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Abstract class for text drawing                                 */
/*------------------------------------------------------------------------*/

#include <exception>

#include "TextContentDrawerJoGL.hxx"
#include "GetJavaProperty.h"
#include "../subwinDrawing/Camera.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
TextContentDrawerJoGL::TextContentDrawerJoGL(DrawableText * drawer)
 :  DrawTextContentStrategy(drawer), DrawableObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  // get pixel bounding in 3D
  double corners[4][3];
  getScreenBoundingBox(corners[0], corners[1], corners[2], corners[3]);

  // convert it to 2D
  Camera * cam = getSubwinDrawer(sciGetParentSubwin(m_pDrawed->getDrawedObject()))->getCamera();
  cam->getSceneCoordinates(corners[0], corner1);
  cam->getSceneCoordinates(corners[1], corner2);
  cam->getSceneCoordinates(corners[2], corner3);
  cam->getSceneCoordinates(corners[3], corner4);
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::getScreenBoundingBox(double corner1[3], double corner2[2], double corner3[2], double corner4[2])
{
  // just update parent figure to avoid problems with OpenGL
	getTextContentDrawerJavaMapper()->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed->getDrawedObject())));

  try
  {
    setDrawerParameters();
  }
  catch (const std::exception & e)
  {
    sciprint(_("%s: No more memory.\n"), "TextContentDrawerJoGL::getPixelBoundingBox");
    return;
  }

  // get text center
  double textCenterPix[3];
  getTextDisplayPos(textCenterPix);

  // convert it to pixels
  Camera * cam = getSubwinDrawer(sciGetParentSubwin(m_pDrawed->getDrawedObject()))->getCamera();
  cam->getPixelCoordinatesRaw(textCenterPix, textCenterPix);

  // we need to convert form OpenGL coordinates to Java ones
  int viewport[4];
  cam->getViewport(viewport);
  textCenterPix[1] = viewport[1] - textCenterPix[1];

  // we got an array of size 12
  double * rect = getTextContentDrawerJavaMapper()->getScreenBoundingBox(textCenterPix[0], textCenterPix[1], textCenterPix[2]);

  corner1[0] = rect[0];
  corner1[1] = viewport[1] - rect[1]; // we need to convert form OpenGL coordinates to Java ones
  corner1[2] = rect[2];

  corner2[0] = rect[3];
  corner2[1] = viewport[1] - rect[4]; // we need to convert form OpenGL coordinates to Java ones
  corner2[2] = rect[5];

  corner3[0] = rect[6];
  corner3[1] = viewport[1] - rect[7]; // we need to convert form OpenGL coordinates to Java ones
  corner3[2] = rect[8];

  corner4[0] = rect[9];
  corner4[1] = viewport[1] - rect[10]; // we need to convert form OpenGL coordinates to Java ones
  corner4[2] = rect[11];

  delete[] rect;
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::drawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  initializeDrawing();
  
  try
  {
    setDrawerParameters();
  }
  catch (const std::exception & e)
  {
    sciprint(_("%s: No more memory.\n"), "TextContentDrawerJoGL::drawTextContent");
    endDrawing();
    return;
  }

  // set text center
  double textPos[3];
  getTextDisplayPos(textPos);
  getTextContentDrawerJavaMapper()->setCenterPosition(textPos[0], textPos[1], textPos[2]);

  sciPointObj * pText = m_pDrawed->getDrawedObject();

  // set box drawing parameters
  if (sciGetIsBoxed(pText))
  {
    getTextContentDrawerJavaMapper()->setBoxDrawingParameters(sciGetIsLine(pText) == TRUE,
                                                              sciGetIsFilled(pText) == TRUE,
                                                              sciGetGraphicContext(pText)->foregroundcolor,
                                                              sciGetGraphicContext(pText)->backgroundcolor);
  }
  else
  {
    getTextContentDrawerJavaMapper()->setBoxDrawingParameters(false, false, 0, 0);
  }

  double * rect = getTextContentDrawerJavaMapper()->drawTextContent();

  convertCornersArray(rect, corner1, corner2, corner3, corner4);

  delete[] rect;
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::redrawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  initializeDrawing();

  // same as draw but we don't need to send parameters to java
  double * rect = getTextContentDrawerJavaMapper()->drawTextContent();
  convertCornersArray(rect, corner1, corner2, corner3, corner4);

  delete[] rect;
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::showTextContent(void)
{
  show();
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::getPixelLength(sciPointObj * parentSubwin, const double startingPoint[3],
                                           double userWidth, double userHeight,
                                           double & pixelWidth, double & pixelHeight )
{
  // get the extreme bound along X axis
  double extremeX[3];
  extremeX[0] = startingPoint[0] + userWidth;
  extremeX[1] = startingPoint[1];
  extremeX[2] = startingPoint[2];

  // get the extreme bound along Y axis
  double extremeY[3];
  extremeY[0] = startingPoint[0];
  extremeY[1] = startingPoint[1] + userHeight;
  extremeY[2] = startingPoint[2];

  Camera * cam = getSubwinDrawer(parentSubwin)->getCamera();

  // convert every one in pixel coordinates
  double textPosPix[3];
  cam->get2dViewPixelCoordinates(startingPoint, textPosPix);
  //sciGetJava2dViewPixelCoordinates(parentSubwin, startingPoint, textPosPix);

  int extremeXPix[3];
  cam->get2dViewPixelCoordinates(extremeX, extremeXPix);
  //sciGetJava2dViewPixelCoordinates(parentSubwin, extremeX, extremeXPix);

  int extremeYPix[3];
  cam->get2dViewPixelCoordinates(extremeY, extremeYPix);
  //sciGetJava2dViewPixelCoordinates(parentSubwin, extremeY, extremeYPix);

  // compute lengths accordingly
  pixelWidth = extremeXPix[0] - textPosPix[0];
  pixelHeight = textPosPix[1] - extremeYPix[1]; // Y axis is inverted
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::getTextDisplayPos(double pos[3])
{
  sciGetTextPos(m_pDrawed->getDrawedObject(), pos);
  m_pDrawed->pointScale(pos[0], pos[1], pos[2], &(pos[0]), &(pos[1]), &(pos[2]) );
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::convertCornersArray(const double corners[12],
                                                double corner1[3],
                                                double corner2[3],
                                                double corner3[3],
                                                double corner4[3])
{
  // we got an array of size 12

  corner1[0] = corners[0];
  corner1[1] = corners[1];
  corner1[2] = corners[2];

  corner2[0] = corners[3];
  corner2[1] = corners[4];
  corner2[2] = corners[5];

  corner3[0] = corners[6];
  corner3[1] = corners[7];
  corner3[2] = corners[8];

  corner4[0] = corners[9];
  corner4[1] = corners[10];
  corner4[2] = corners[11];

  // use logarithmic scale if needed.
  m_pDrawed->inversePointScale(corner1[0], corner1[1], corner1[2],
    &(corner1[0]), &(corner1[1]), &(corner1[2]));
  m_pDrawed->inversePointScale(corner2[0], corner2[1], corner2[2],
    &(corner2[0]), &(corner2[1]), &(corner2[2]));
  m_pDrawed->inversePointScale(corner3[0], corner3[1], corner3[2],
    &(corner3[0]), &(corner3[1]), &(corner3[2]));
  m_pDrawed->inversePointScale(corner4[0], corner4[1], corner4[2],
    &(corner4[0]), &(corner4[1]), &(corner4[2]));
}
/*---------------------------------------------------------------------------------*/
void TextContentDrawerJoGL::getUserSizePix(double & boxWidthPix, double & boxHeightPix)
{
  // get box size in user coordinates
  sciPointObj * pObj = m_pDrawed->getDrawedObject();

  double boxWidth;
  double boxHeight;
  sciGetUserSize(pObj, &boxWidth, &boxHeight);

  double textPos[3];
  sciGetTextPos(pObj, textPos);

  // convert the user lengths to pixel ones.
  getPixelLength(sciGetParentSubwin(pObj), textPos, boxWidth, boxHeight, boxWidthPix, boxHeightPix);
}
/*---------------------------------------------------------------------------------*/
TextContentDrawerJavaMapper * TextContentDrawerJoGL::getTextContentDrawerJavaMapper(void)
{
  return dynamic_cast<TextContentDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
