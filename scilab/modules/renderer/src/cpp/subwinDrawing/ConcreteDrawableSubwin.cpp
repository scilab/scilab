/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableSubwin.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing drivers independent routines for a subwin      */
/*        object                                                          */
/*------------------------------------------------------------------------*/

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
}
/*------------------------------------------------------------------------------------------*/
ConcreteDrawableSubwin::~ConcreteDrawableSubwin(void)
{
  delete m_pXBoundsStrategy;
  m_pXBoundsStrategy = NULL;
  delete m_pYBoundsStrategy;
  m_pYBoundsStrategy = NULL;
  delete m_pZBoundsStrategy;
  m_pZBoundsStrategy = NULL;

  if (m_pAxesBoxDrawer != NULL)
  {
    delete m_pAxesBoxDrawer;
    m_pAxesBoxDrawer = NULL;
  }
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
  sciGetDataBounds(m_pDrawed, userBounds);

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
  // TODO
}
/*------------------------------------------------------------------------------------------*/

}
