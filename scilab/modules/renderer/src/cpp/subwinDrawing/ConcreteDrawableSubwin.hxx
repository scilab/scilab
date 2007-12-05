/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableSubwin.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing drivers independent routines for a subwin      */
/*        object                                                          */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_SUBWIN_HXX_
#define _CONCRETE_DRAWABLE_SUBWIN_HXX_

#include "DrawableSubwin.h"
#include "ComputeBoundsStrategy.hxx"
#include "DrawAxesBoxStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableSubwin : public DrawableSubwin
{
public:

  ConcreteDrawableSubwin(sciPointObj * pObj);

  virtual ~ConcreteDrawableSubwin(void);

  void setXBoundsStrategy(ComputeBoundsStrategy * strategy);
  void setYBoundsStrategy(ComputeBoundsStrategy * strategy);
  void setZBoundsStrategy(ComputeBoundsStrategy * strategy);

  void setAxesBoxDrawer(DrawAxesBoxStrategy * strategy);

  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double xCoord, double yCoord, double zCoord,
                          double * xScaled, double * yScaled, double * zScaled);


  /**
   * Inverse function of pointScale.
   */
  virtual void inversePointScale(double xScaled, double yScaled, double zScaled,
                                 double * xCoord, double * yCoord, double * zCoord);

  /**
   * Apply scale on a vector.
   */
  virtual void pointScale(double vectorX[], double vectorY[], double vectorZ[], int vectorLength);

  /**
   * Apply non linear scaling on direction vector (opposite as point vector).
   * Since scaling is non linear, we need to have a starting point of the vector.
   */
  virtual void directionScale(double xCoord, double yCoord, double zCoord,
                              double startingPointX, double startingPointY, double startingPointZ,
                              double * xScaled, double * yScaled, double * zScaled);

  /**
   * Apply distance scale on a vector.
   */
  virtual void directionScale(double vectorX[], double vectorY[], double vectorZ[],
                              double startingPointsX[], double startingPointsY[],
                              double startingPointsZ[], double vectorLength);

protected:

  /*---------------------------------------------------------------------*/
  /**
   * Compute real data bounds from the user ones.
   * ie fills Frect.
   */
  virtual void computeRealDataBounds(void);

  /**
   * Draw the surrounding cube of the subwin object.
   */
  virtual void drawBox(void);

  /**
  * Draw the ticks of the subwin object.
  */
  virtual void drawTicks(void);
  /*---------------------------------------------------------------------*/
  ComputeBoundsStrategy * m_pXBoundsStrategy;
  ComputeBoundsStrategy * m_pYBoundsStrategy;
  ComputeBoundsStrategy * m_pZBoundsStrategy;

  DrawAxesBoxStrategy * m_pAxesBoxDrawer;
  /*---------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_SUBWIN_HXX_ */
