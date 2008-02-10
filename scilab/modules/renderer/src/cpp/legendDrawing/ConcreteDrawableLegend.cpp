/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableLegend.cpp                                       */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independant routine to draw a       */
/*        legend object                                                   */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "ConcreteDrawableLegend.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "MALLOC.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
ConcreteDrawableLegend::ConcreteDrawableLegend(sciPointObj * pLegend)
  : DrawableLegend(pLegend)
{
  m_pNames = NULL;
  m_aLines = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableLegend::~ConcreteDrawableLegend(void)
{
  destroyText();
  destroyLines();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::drawLegend(void)
{
  setTextParameters();
  createLines();
  setLinesParameters();

  double corner1[3];
  double corner2[3];
  double corner3[3];
  double corner4[3];

  // compute the box in which to put the legend
  getLineBox(corner1, corner2, corner3, corner4);

  placeText(corner1, corner2, corner3, corner4);
  placeLines(corner1, corner2, corner3, corner4);

  // draw names
  getHandleDrawer(m_pNames)->hasChanged();
  getHandleDrawer(m_pNames)->display();
  
  // draw lines
  int nbLegends = getNbLegend();
  for (int i = 0; i < nbLegends; i++)
  {
    getHandleDrawer(m_aLines[i])->hasChanged();
    getHandleDrawer(m_aLines[i])->display();
  }

}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::showLegend(void)
{
  // just do that in order to be able to retrieve bounding box
  sciSetTextPos(m_pNames, 1.0, 1.0, 1.0);
  // line parameters may have changed
  setLinesParameters();

  // position always change
  double corner1[3];
  double corner2[3];
  double corner3[3];
  double corner4[3];

  // compute the box in which to put the legend
  getLineBox(corner1, corner2, corner3, corner4);
  placeText(corner1, corner2, corner3, corner4);
  placeLines(corner1, corner2, corner3, corner4);

  // draw names
  getHandleDrawer(m_pNames)->hasChanged();
  getHandleDrawer(m_pNames)->display();

  // draw lines
  int nbLegends = getNbLegend();
  for (int i = 0; i < nbLegends; i++)
  {
    getHandleDrawer(m_aLines[i])->hasChanged();
    getHandleDrawer(m_aLines[i])->display();
  }

}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::setTextParameters(void)
{
  destroyText();

  sciLegend * ppLegend = pLEGEND_FEATURE(m_pDrawed);
  int nbLegends = ppLegend->nblegends;

  // get the text of the legend
  // use malloc since it will destroyed by FREE in deallocate text
  char ** textMat = new char *[nbLegends];
  computeTextMatrix(textMat);


  // create a text object and set its data from the legend object
  int background = sciGetBackground(m_pDrawed);
  int foreground = sciGetForeground(m_pDrawed);

  m_pNames = allocateText(sciGetParentSubwin(m_pDrawed), textMat, nbLegends, 1,
                          0.0, 0.0, TRUE, NULL, FALSE, &foreground, &background,
                          FALSE, FALSE, FALSE, ALIGN_LEFT);

  // textMat has been copied
  for (int i = 0; i < nbLegends; i++)
  {
    delete[] textMat[i];
  }
  delete[] textMat;

  sciInitFontSize(m_pNames, sciGetFontSize(m_pDrawed));
  sciInitFontForeground(m_pNames, sciGetForeground(m_pDrawed));
  sciInitIsClipping(m_pNames, sciGetIsClipping(m_pDrawed));
  sciSetClipping(m_pNames, sciGetClipping(m_pDrawed));

  sciSetTextPos(m_pNames, 1.0, 1.0, 1.0);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::createLines(void)
{
  // create nbLegends polylines
  destroyLines();
  int nbLegends = pLEGEND_FEATURE(m_pDrawed)->nblegends;
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pDrawed);
  double defaultCoords[3] = {0.0, 0.0, 0.0};
  int defColor = 0;

  m_aLines = new sciPointObj *[nbLegends];

  for (int i = 0; i < nbLegends; i++)
  {
    m_aLines[i] = allocatePolyline(parentSubwin, defaultCoords, defaultCoords, defaultCoords, 0, 3, 1,
                                   NULL, NULL, NULL, NULL, NULL, FALSE, FALSE, FALSE, FALSE);
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::setLinesParameters(void)
{
  int nbLegends = getNbLegend();
  sciLegend * ppLegend = pLEGEND_FEATURE(m_pDrawed);
  for (int i = 0; i < nbLegends; i++)
  {
    // current object we want to set the legend
    sciPointObj * legendedObject = ppLegend->pptabofpointobj[i];

    // set same mark parameters as the polyline
    sciInitMarkSize(m_aLines[i], sciGetMarkSize(legendedObject));
    sciInitMarkSizeUnit(m_aLines[i], sciGetMarkSizeUnit(legendedObject));
    sciInitMarkForeground(m_aLines[i], sciGetMarkForeground(legendedObject));
    sciInitMarkBackground(m_aLines[i], sciGetMarkBackground(legendedObject));
    sciInitIsMark(m_aLines[i], sciGetIsMark(legendedObject));

    // same for lines
    sciInitForeground(m_aLines[i], sciGetForeground(legendedObject));
    sciInitLineWidth(m_aLines[i], sciGetLineWidth(legendedObject));
    sciInitLineStyle(m_aLines[i], sciGetLineStyle(legendedObject));
    sciInitIsLine(m_aLines[i], sciGetIsLine(legendedObject));

    // use clip state of legend object
    sciInitIsClipping(m_aLines[i], sciGetIsClipping(m_pDrawed));
    sciSetClipping(m_aLines[i], sciGetClipping(m_pDrawed));
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::computeTextMatrix(char ** textMat)
{
  sciLegend * ppLegend = pLEGEND_FEATURE(m_pDrawed);
  int nbLegends = ppLegend->nblegends;

  // get the text contained in the legend
  char * arobText = getStrMatElement(sciGetText(m_pDrawed),0,0);
  int arobaseIndex = 0;
  int prevIndex = 0;
  // don't use strtok, function unsafe
  for (int i = 0; i < nbLegends; i++) {
    // find next token
    while( arobText[arobaseIndex] != '@' && arobText[arobaseIndex] != 0)
    {
      arobaseIndex++;
    }

    // we found a token
    // then copy everything between prevIndex and arobase index
    int stringLength = arobaseIndex - prevIndex;
    textMat[i] = new char[stringLength + 1]; //  +1 for null terminating character
    for (int j = 0; j < stringLength; j++) {
      textMat[i][j] = arobText[prevIndex + j];
    }
    textMat[i][stringLength] = 0;

    // skip the @
    arobaseIndex++;
    prevIndex = arobaseIndex;
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::destroyLines(void)
{
  if (m_aLines != NULL)
  {

    for (int i = 0; i < pLEGEND_FEATURE(m_pDrawed)->nblegends; i++)
    {
      deallocatePolyline(m_aLines[i]);
      m_aLines[i] = NULL;
    }
    delete[] m_aLines;
    m_aLines = NULL;
  }
 
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::destroyText(void)
{
  if (m_pNames != NULL)
  {
    deallocateText(m_pNames);
    m_pNames = NULL;
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::getLineBox(double upperLeftCorner[3], double lowerLeftCorner[3],
                                        double lowerRightCorner[3], double upperRightCorner[3])
{
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pNames);
  // get viewing area used by axes
  int axesXpos;
  int axesYpos;
  int axesWidth;
  int axesHeight;
  sciGetViewingArea(parentSubwin, &axesXpos, &axesYpos, &axesWidth, &axesHeight);

  // get text bounding box
  int corner1[2];
  int corner2[2];
  int corner3[2];
  int corner4[2];
  sciGetPixelBoundingBox(m_pNames, corner1, corner2, corner3, corner4);

  // the height of the box equals the text height
   double boxPixelHeight = Abs(corner1[1] - corner3[1]);

  // width in pixel of the box
  double boxPixelWidth = axesWidth / 10.0;

  // we need to fin a suitable z pixel coordinate which won't be clipped
  // by near or far plane. We chose one of the axes bounds.
  double axesBounds[6];
  sciGetDataBounds(parentSubwin, axesBounds);
  // find a point on the axes
  // xmin, ymin, zmin
  double pointOnAxes[3] = {axesBounds[0], axesBounds[2], axesBounds[4]};
  // find it pixel coordinates
  getSubwinDrawer(parentSubwin)->getCamera()->getPixelCoordinates(pointOnAxes, pointOnAxes);
  // get the Z coordinate
  double pixelDepth = pointOnAxes[2];

  // we got all the information we need, we can compute the 4 corners
  // upper left point
  // just behind axes box, that's why the 1.05
  double pixCorner1[3] = {axesXpos, axesYpos + axesHeight * 1.05, pixelDepth};

  // lower left point
  double pixCorner2[3] = {pixCorner1[0], pixCorner1[1] + boxPixelHeight, pixelDepth};

  // lower right point
  double pixCorner3[3] = {pixCorner1[0] + boxPixelWidth, pixCorner2[1], pixelDepth};

  // upper right point
  double pixCorner4[3] = {pixCorner3[0], pixCorner1[1],pixelDepth };

  // convert everything to scene coordinates
  Camera * curCam = getSubwinDrawer(parentSubwin)->getCamera();
  curCam->getSceneCoordinates(pixCorner1, upperLeftCorner);
  curCam->getSceneCoordinates(pixCorner2, lowerLeftCorner);
  curCam->getSceneCoordinates(pixCorner3, lowerRightCorner);
  curCam->getSceneCoordinates(pixCorner4, upperRightCorner);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::placeLines(const double upperLeftCorner[3], const double lowerLeftCorner[3],
                                        const double lowerRightCorner[3], const double upperRightCorner[3])
{
  int nblegends = getNbLegend();

  // interpolate between top to bottom
  for (int i = 0; i < nblegends; i++)
  {
    // our polylines are composed of 3 data
    sciPolyline * curPoly = pPOLYLINE_FEATURE(m_aLines[i]);

    // interpolate on left side
    double interpolationFactor = (i + 0.5) / nblegends;
    curPoly->pvx[0] = lowerLeftCorner[0] + (upperLeftCorner[0] - lowerLeftCorner[0]) * interpolationFactor;
    curPoly->pvy[0] = lowerLeftCorner[1] + (upperLeftCorner[1] - lowerLeftCorner[1]) * interpolationFactor;
    curPoly->pvz[0] = lowerLeftCorner[2] + (upperLeftCorner[2] - lowerLeftCorner[2]) * interpolationFactor;

    // then on right side
    curPoly->pvx[2] = lowerRightCorner[0] + (upperRightCorner[0] - lowerRightCorner[0]) * interpolationFactor;
    curPoly->pvy[2] = lowerRightCorner[1] + (upperRightCorner[1] - lowerRightCorner[1]) * interpolationFactor;
    curPoly->pvz[2] = lowerRightCorner[2] + (upperRightCorner[2] - lowerRightCorner[2]) * interpolationFactor;

    // middle of the two others
    curPoly->pvx[1] = 0.5 * (curPoly->pvx[0] + curPoly->pvx[2]);
    curPoly->pvy[1] = 0.5 * (curPoly->pvy[0] + curPoly->pvy[2]);
    curPoly->pvz[1] = 0.5 * (curPoly->pvz[0] + curPoly->pvz[2]);
  }

}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableLegend::placeText(const double upperLeftCorner[3], const double lowerLeftCorner[3],
                                       const double lowerRightCorner[3], const double upperRightCorner[3])
{
  // use the lowerLeftcorner, but move a little to the right
  double xOffset = (lowerRightCorner[0] - lowerLeftCorner[0]) * 0.2;
  sciSetTextPos(m_pNames, lowerRightCorner[0] + xOffset,
                lowerRightCorner[1], lowerRightCorner[2]);
}
/*---------------------------------------------------------------------------------*/
}
