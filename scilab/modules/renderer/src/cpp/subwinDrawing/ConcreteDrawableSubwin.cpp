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

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
ConcreteDrawableSubwin::ConcreteDrawableSubwin(sciPointObj * pObj)
  : DrawableSubwin(pObj)
{
  m_pXBoundsStrategy = NULL;
  m_pYBoundsStrategy = NULL;
  m_pZBoundsStrategy = NULL;
  m_pAxesBoxDrawer = NULL;
  m_pXTicksDrawer = NULL;
  m_pYTicksDrawer = NULL;
  m_pZTicksDrawer = NULL;
}
/*------------------------------------------------------------------------------------------*/
ConcreteDrawableSubwin::~ConcreteDrawableSubwin(void)
{

  setXBoundsStrategy(NULL);
  setYBoundsStrategy(NULL);
  setZBoundsStrategy(NULL);

  setAxesBoxDrawer(NULL);

  setXTicksDrawer(NULL);
  setYTicksDrawer(NULL);
  setZTicksDrawer(NULL);
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
void ConcreteDrawableSubwin::setAxesBoxDrawer(DrawAxesBoxStrategy * strategy)
{
  if (m_pAxesBoxDrawer != NULL)
  {
    delete m_pAxesBoxDrawer;
  }
  m_pAxesBoxDrawer = strategy;
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
  if (!sciGetTightLimitsOn(m_pDrawed))
  {
    m_pXBoundsStrategy->applyBestFitting(bestXBounds, bestXBounds);
    m_pYBoundsStrategy->applyBestFitting(bestYBounds, bestYBounds);
    m_pZBoundsStrategy->applyBestFitting(bestZBounds, bestZBounds);
  }

  sciSetRealDataBounds(m_pDrawed, bestBounds);

}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::drawBox(void)
{
  // If axes is not displayed m_pAxesbox is not drawn.
  if (m_pAxesBoxDrawer != NULL)
  {
    m_pAxesBoxDrawer->drawAxesBox();
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::drawTicks(void)
{
  double distToXaxis = 0.0;
  double distToYaxis = 0.0;
  double distToZaxis = 0.0;
  if (m_pXTicksDrawer != NULL)
  {
    distToXaxis = m_pXTicksDrawer->draw();
  }
  if (m_pYTicksDrawer != NULL)
  {
    distToYaxis = m_pYTicksDrawer->draw();
  }
  if (m_pZTicksDrawer != NULL)
  {
    distToZaxis = m_pZTicksDrawer->draw();
  }

  /* for title there is no displayable ticks */
  setLabelsDistanceToAxis(distToXaxis, distToYaxis, distToZaxis, 0.0);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableSubwin::showBox(void)
{
  // If axes is not displayed m_pAxesbox is not drawn.
  if (m_pAxesBoxDrawer != NULL)
  {
    m_pAxesBoxDrawer->show(); 
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
    distToXaxis = m_pXTicksDrawer->showTicks();
  }
  if (m_pYTicksDrawer != NULL)
  {
    distToYaxis = m_pYTicksDrawer->showTicks();
  }
  if (m_pZTicksDrawer != NULL)
  {
    distToZaxis = m_pZTicksDrawer->showTicks();
  }

  /* for title there is no displayable ticks */
  setLabelsDistanceToAxis(distToXaxis, distToYaxis, distToZaxis, 0.0);

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

}
