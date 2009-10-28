/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing drivers independent routines for a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableSubwin.hxx"
#include "getHandleDrawer.h"
#include "BasicAlgos.hxx"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
#include "pixel_mode.h"
#include "DrawingBridge.h"
#include "HandleManagement.h"
}

using namespace std;

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
ConcreteDrawableSubwin::ConcreteDrawableSubwin(sciPointObj * pObj)
  : DrawableSubwin(pObj)
{
  m_pXBoundsStrategy = NULL;
  m_pYBoundsStrategy = NULL;
  m_pZBoundsStrategy = NULL;
  m_pXTicksDrawer = NULL;
  m_pYTicksDrawer = NULL;
  m_pZTicksDrawer = NULL;

  m_oAxesBoxDrawers.clear();

}
/*------------------------------------------------------------------------------------------*/
ConcreteDrawableSubwin::~ConcreteDrawableSubwin(void)
{

  setXBoundsStrategy(NULL);
  setYBoundsStrategy(NULL);
  setZBoundsStrategy(NULL);

  setXTicksDrawer(NULL);
  setYTicksDrawer(NULL);
  setZTicksDrawer(NULL);

  removeAxesBoxDrawers();

  m_oDisplayedTexts.clear();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setXBoundsStrategy(ComputeBoundsStrategy * strategy)
{
  if (m_pXBoundsStrategy != NULL)
  {
    delete m_pXBoundsStrategy;
  }
  m_pXBoundsStrategy = strategy;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setYBoundsStrategy(ComputeBoundsStrategy * strategy)
{
  if (m_pYBoundsStrategy != NULL)
  {
    delete m_pYBoundsStrategy;
  }
  m_pYBoundsStrategy = strategy;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setZBoundsStrategy(ComputeBoundsStrategy * strategy)
{
  if (m_pZBoundsStrategy != NULL)
  {
    delete m_pZBoundsStrategy;
  }
  m_pZBoundsStrategy = strategy;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setXTicksDrawer(TicksDrawer * xTicksDrawer)
{
  if(m_pXTicksDrawer != NULL)
  {
    delete m_pXTicksDrawer;
  }
  m_pXTicksDrawer = xTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setYTicksDrawer(TicksDrawer * yTicksDrawer)
{
  if(m_pYTicksDrawer != NULL)
  {
    delete m_pYTicksDrawer;
  }
  m_pYTicksDrawer = yTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setZTicksDrawer(TicksDrawer * zTicksDrawer)
{
  if(m_pZTicksDrawer != NULL)
  {
    delete m_pZTicksDrawer;
  }
  m_pZTicksDrawer = zTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::addAxesBoxDrawer(DrawAxesBoxStrategy * strategy)
{
  m_oAxesBoxDrawers.push_back(strategy);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::removeAxesBoxDrawers(void)
{
  list<DrawAxesBoxStrategy *>::iterator it = m_oAxesBoxDrawers.begin();
  for( ; it != m_oAxesBoxDrawers.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oAxesBoxDrawers.clear();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::pointScale(double xCoord, double yCoord, double zCoord,
                                        double * xScaled, double * yScaled, double * zScaled)
{
  if (xScaled != NULL) { m_pXBoundsStrategy->pointScale(xCoord, xScaled); }
  if (yScaled != NULL) { m_pYBoundsStrategy->pointScale(yCoord, yScaled); }
  if (zScaled != NULL) { m_pZBoundsStrategy->pointScale(zCoord, zScaled); }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::inversePointScale(double xScaled, double yScaled, double zScaled,
                                               double * xCoord, double * yCoord, double * zCoord)
{
  if (xCoord != NULL) { m_pXBoundsStrategy->inversePointScale(xScaled, xCoord); }
  if (yCoord != NULL) { m_pYBoundsStrategy->inversePointScale(yScaled, yCoord); }
  if (zCoord != NULL) { m_pZBoundsStrategy->inversePointScale(zScaled, zCoord); }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::pointScale(double vectorX[], double vectorY[], double vectorZ[], int vectorLength)
{
  if (vectorX != NULL) { m_pXBoundsStrategy->pointScale(vectorX, vectorLength); }
  if (vectorY != NULL) { m_pYBoundsStrategy->pointScale(vectorY, vectorLength); }
  if (vectorZ != NULL) { m_pZBoundsStrategy->pointScale(vectorZ, vectorLength); }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::directionScale(double xCoord, double yCoord, double zCoord,
                                            double startingPointX, double startingPointY, double startingPointZ,
                                            double * xScaled, double * yScaled, double * zScaled)
{
  if (xScaled != NULL) { m_pXBoundsStrategy->directionScale(xCoord, startingPointX, xScaled); }
  if (yScaled != NULL) { m_pYBoundsStrategy->directionScale(yCoord, startingPointY, yScaled); }
  if (zScaled != NULL) { m_pZBoundsStrategy->directionScale(zCoord, startingPointZ, zScaled); }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::directionScale(double vectorX[], double vectorY[], double vectorZ[],
                                            double startingPointsX[], double startingPointsY[],
                                            double startingPointsZ[], double vectorLength)
{
  if (vectorX != NULL) { m_pXBoundsStrategy->directionScale(vectorX, startingPointsX, vectorLength); }
  if (vectorY != NULL) { m_pYBoundsStrategy->directionScale(vectorY, startingPointsY, vectorLength); }
  if (vectorZ != NULL) { m_pZBoundsStrategy->directionScale(vectorZ, startingPointsZ, vectorLength); }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::computeRealDataBounds(void)
{
  // retrieve user bounds
  double userBounds[6];
  sciGetDisplayedDataBounds(m_pDrawed, userBounds);

  // check wether the subwin is zoomed or not
  bool isZoomed = (sciGetZooming(m_pDrawed) == TRUE);

  double bestBounds[6]; // output bounds

  // take sub arrays
  double * userXBounds = userBounds;
  double * userYBounds = userBounds + 2;
  double * userZBounds = userBounds + 4;

  double * bestXBounds = bestBounds;
  double * bestYBounds = bestBounds + 2;
  double * bestZBounds = bestBounds + 4;

  // apply scale
  m_pXBoundsStrategy->applyScaleModification(userXBounds, bestXBounds);
  m_pYBoundsStrategy->applyScaleModification(userYBounds, bestYBounds);
  m_pZBoundsStrategy->applyScaleModification(userZBounds, bestZBounds);

  // fit them if needed
  // for a more accurate zoom, tigth limits are enable if the zoom
  // is enable
  if (!sciGetTightLimitsOn(m_pDrawed) && !isZoomed)
  {
    m_pXBoundsStrategy->applyBestFitting(bestXBounds, bestXBounds);
    m_pYBoundsStrategy->applyBestFitting(bestYBounds, bestYBounds);
    m_pZBoundsStrategy->applyBestFitting(bestZBounds, bestZBounds);
  }

  sciSetRealDataBounds(m_pDrawed, bestBounds);

}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::updateScale(void)
{

  if (!m_bNeedCoordUpdate) {
    // no need to update
    return;
  }

	// to be sure that the inner structure is up to date.
	update();

  sciPointObj * parentFigure = sciGetParentFigure(m_pDrawed);
  BOOL visible = sciGetVisibility(m_pDrawed);
  int pixelMode = sciGetXorMode(parentFigure);


  // update the data by just calling
  // display on the invisible window
  sciSetXorMode(parentFigure, getPixelModeIndex("noop"));
  sciSetVisibility(m_pDrawed, FALSE);
  sciDrawSingleObj(m_pDrawed);
  sciSetVisibility(m_pDrawed, visible);
  sciSetXorMode(parentFigure, pixelMode);
}
/*------------------------------------------------------------------------------------------*/
int ConcreteDrawableSubwin::getNbXTicks(void)
{
	// to be sure that the inner structure is up to date.
	update();

  if (m_pXTicksDrawer != NULL)
  {
    return m_pXTicksDrawer->getInitNbTicks();
  }
  else
  {
    return 0;
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::getXTicksPos(double ticksPositions[], char ** ticksLabels)
{
	// to be sure that the inner structure is up to date.
	update();

  if (m_pXTicksDrawer != NULL)
  {
    m_pXTicksDrawer->getInitTicksPos(ticksPositions, ticksLabels);

    // revert log scale if needed
    m_pXBoundsStrategy->inversePointScale(ticksPositions, getNbXTicks());
  }
}
/*------------------------------------------------------------------------------------------*/
int ConcreteDrawableSubwin::getNbYTicks(void)
{
	// to be sure that the inner structure is up to date.
	update();

  if (m_pYTicksDrawer != NULL)
  {
    return m_pYTicksDrawer->getInitNbTicks();
  }
  else
  {
    return 0;
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::getYTicksPos(double ticksPositions[], char ** ticksLabels)
{
	// to be sure that the inner structure is up to date.
	update();

  if (m_pYTicksDrawer != NULL)
  {
    m_pYTicksDrawer->getInitTicksPos(ticksPositions, ticksLabels);

    // revert log scale if needed
    m_pYBoundsStrategy->inversePointScale(ticksPositions, getNbYTicks());
  }
}
/*------------------------------------------------------------------------------------------*/
int ConcreteDrawableSubwin::getNbZTicks(void)
{
	// to be sure that the inner structure is up to date.
	update();

  if (m_pZTicksDrawer != NULL)
  {
    return m_pZTicksDrawer->getInitNbTicks();
  }
  else
  {
    return 0;
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::getZTicksPos(double ticksPositions[], char ** ticksLabels)
{
	// to be sure that the inner structure is up to date.
	update();

  if (m_pZTicksDrawer != NULL)
  {
    m_pZTicksDrawer->getInitTicksPos(ticksPositions, ticksLabels);

    // revert log scale if needed
    m_pZBoundsStrategy->inversePointScale(ticksPositions, getNbZTicks());
  }
}
/*------------------------------------------------------------------------------------------*/
bool ConcreteDrawableSubwin::getXAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3])
{

  if (m_pXTicksDrawer != NULL)
  {
    m_pXTicksDrawer->getAxisPosition(axisStart, axisEnd, ticksDirection);
    return true;
  }
  else
  {
    return false;
  }
}
/*------------------------------------------------------------------------------------------*/
bool ConcreteDrawableSubwin::getYAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3])
{
  if (m_pYTicksDrawer != NULL)
  {
    m_pYTicksDrawer->getAxisPosition(axisStart, axisEnd, ticksDirection);
    return true;
  }
  else
  {
    return false;
  }
}
/*------------------------------------------------------------------------------------------*/
bool ConcreteDrawableSubwin::getZAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3])
{
  if (m_pZTicksDrawer != NULL)
  {
    m_pZTicksDrawer->getAxisPosition(axisStart, axisEnd, ticksDirection);
    return true;
  }
  else
  {
    return false;
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::addTextToDraw(sciPointObj * text)
{
	m_oDisplayedTexts.push_back(text);
	textChanged();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::removeTextToDraw(sciPointObj * text)
{
	m_oDisplayedTexts.remove(text);
  textChanged();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::displayChildren(void)
{
  // draw the children as usual
  DrawableObject::displayChildren();

  // draw the text after
  displayTexts();

}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::textChanged(void)
{
  m_bTextListChanged = true;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::drawBox(void)
{
  // If axes is not displayed m_pAxesbox is not drawn.
  if (m_oAxesBoxDrawers.empty())
  {
    return ;
  }

  int concealedCornerIndex = computeConcealedCornerIndex();
  list<DrawAxesBoxStrategy *>::iterator it = m_oAxesBoxDrawers.begin();
  for ( ; it != m_oAxesBoxDrawers.end(); it++)
  {
    (*it)->drawAxesBox(concealedCornerIndex);
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::drawTicks(void)
{
  double distToXaxis = 0.0;
  double distToYaxis = 0.0;
  double distToZaxis = 0.0;
  
  // Z ticks are deeper qo draw them before
  if (m_pZTicksDrawer != NULL)
  {
    distToZaxis = m_pZTicksDrawer->draw();
  }
  if (m_pYTicksDrawer != NULL)
  {
    distToYaxis = m_pYTicksDrawer->draw();
  }
  if (m_pXTicksDrawer != NULL)
  {
    distToXaxis = m_pXTicksDrawer->draw();
  }

  /* for title there is no displayable ticks */
  setLabelsDistanceToAxis(distToXaxis, distToYaxis, distToZaxis, 0.0);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::showBox(void)
{
  list<DrawAxesBoxStrategy *>::iterator it = m_oAxesBoxDrawers.begin();
  for ( ; it != m_oAxesBoxDrawers.end(); it++)
  {
    (*it)->show();
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::showTicks(void)
{
  double distToXaxis = 0.0;
  double distToYaxis = 0.0;
  double distToZaxis = 0.0;
  if (m_pXTicksDrawer != NULL)
  {
    distToXaxis = m_pXTicksDrawer->show();
  }
  if (m_pYTicksDrawer != NULL)
  {
    distToYaxis = m_pYTicksDrawer->show();
  }
  if (m_pZTicksDrawer != NULL)
  {
    distToZaxis = m_pZTicksDrawer->show();
  }

  /* for title there is no displayable ticks */
  setLabelsDistanceToAxis(distToXaxis, distToYaxis, distToZaxis, 0.0);

}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::displayLabels(void)
{
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  
  // there are 4 labels, stored at then end of the list
  for (int i = 0; i < 4; i++)
  {
    getHandleDrawer(curSon->pointobj)->display();
    curSon = curSon->pprev;
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::displayTexts(void)
{
 
  // sortDisplayed text if needed
  if (m_bNeedDraw || m_bNeedRedraw || m_bTextListChanged)
  {
    sortDisplayedTexts();
  }

  // display all the text registered in the list
  // The list should be sorted
	list<sciPointObj *>::iterator it = m_oDisplayedTexts.begin();
  for ( ; it != m_oDisplayedTexts.end(); it++)
  {
    // HACK here. This patch is to force disepearance
    // of text objects if one of there parents is not visible.
    if (sciGetRealVisibility(*it))
    {
      getHandleDrawer(*it)->display();
    }
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::sortDisplayedTexts(void)
{
	// sort the text from back to front
	m_oDisplayedTexts.sort(getTextOrder);

  // text has been sorted successfully
  m_bTextListChanged = false;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::setLabelsDistanceToAxis(double xLabelDist, double yLabelDist,
                                                     double zLabelDist, double titleDist)
{
  sciPointObj * xLabel = pSUBWIN_FEATURE(m_pDrawed)->mon_x_label;
  getLabelDrawer(xLabel)->setDistanceToAxis(xLabelDist);

  sciPointObj * yLabel = pSUBWIN_FEATURE(m_pDrawed)->mon_y_label;
  getLabelDrawer(yLabel)->setDistanceToAxis(yLabelDist);

  sciPointObj * zLabel = pSUBWIN_FEATURE(m_pDrawed)->mon_z_label;
  getLabelDrawer(zLabel)->setDistanceToAxis(zLabelDist);

  sciPointObj * titleLabel = pSUBWIN_FEATURE(m_pDrawed)->mon_title;
  getLabelDrawer(titleLabel)->setDistanceToAxis(titleDist);
}
/*------------------------------------------------------------------------------------------*/
int ConcreteDrawableSubwin::computeConcealedCornerIndex(void)
{
  double bounds[6];
  sciGetRealDataBounds(m_pDrawed, bounds);

  Camera * cam = getCamera();

  // eight conrners of the axes box
  double corners[8][3];
  for (int i = 0; i < 8; i++)
  {
    // compute position of corner i
    corners[i][0] = (i < 4 ? bounds[0] : bounds[1]); // xMin or xMax
    corners[i][1] = ((i % 4) < 2 ? bounds[2] : bounds[3]); // yMin or yMax
    corners[i][2] = ((i % 2) == 0 ? bounds[4] : bounds[5]); // zMin or zMax

    // directly convert it to pixel coordinates
    cam->getPixelCoordinatesRaw(corners[i], corners[i]);
  }

  // fin the index of the deeper corner
  double eyeDistance = corners[0][2];
  int farthestCornerIndex = 0;
  for (int i = 1; i < 8; i++) {
    if (corners[i][2] > eyeDistance) {
      eyeDistance = corners[i][2];
      farthestCornerIndex = i;
    }
  }
  return farthestCornerIndex;

}
/*---------------------------------------------------------------------------------*/
double ConcreteDrawableSubwin::getEyeDistance(Camera * cam, sciPointObj * pText)
{
	// text is drawn flat so any point of the text has the same distance
  // with the viewpoint. So let choose the text center.
  double textPos[3];
  sciGetTextPos(pText, textPos);

  // convert it to pixel coordinate. Z value correspond to the depth.
  cam->getPixelCoordinates(textPos, textPos);

  return textPos[2];
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableSubwin::getTextOrder(sciPointObj * pText1, sciPointObj * pText2)
{
	Camera * cam = getSubwinDrawer(sciGetParentSubwin(pText1))->getCamera();
	// find the deepest one
	return (getEyeDistance(cam, pText1) > getEyeDistance(cam, pText2));
}
/*---------------------------------------------------------------------------------*/
}
