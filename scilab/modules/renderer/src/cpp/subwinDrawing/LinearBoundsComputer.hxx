/*------------------------------------------------------------------------*/
/* file: LinearBoundsComputer.hxx                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for computing best fitting linear bounds from user     */
/*        ones                                                            */
/*------------------------------------------------------------------------*/

#ifndef _LINEAR_BOUNDS_COMPUTER_HXX_
#define _LINEAR_BOUNDS_COMPUTER_HXX_

#include "ComputeBoundsStrategy.hxx"

namespace sciGraphics
{

class LinearBoundsComputer : public ComputeBoundsStrategy
{
public:

  LinearBoundsComputer(void) : ComputeBoundsStrategy() {}

  virtual ~LinearBoundsComputer(void) {}

  /**
   * Modify scale for (to use with non linear scale).
   */
  virtual void applyScaleModification(const double inputBounds[2], double outputBounds[2]);

  /**
   * Modify data bounds in order to have nice ticks labels (not 0.9545121 for example).
   */
  virtual void applyBestFitting(const double inputBounds[2], double outputBounds[2]);

  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double coord, double * scaledCoord);

  /**
   * Inverse of pointScale.
   */
  virtual void inversePointScale(double scaledCoord, double * coord);

  /**
   * Apply scale on a vector.
   */
  virtual void pointScale(double vector[], int vectorLength);

  /**
   * Apply non linear scaling on direction vector (opposite as point vector).
   * Since scaling is non linear, we need to have a starting point of the vector.
   */
  virtual void directionScale(double coord,  double startingPoint, double * scaledCoord);

  /**
   * Apply distance scale on a vector.
   */
  virtual void directionScale(double vector[], double startingPoints[], double vectorLength);

protected:

};

}

#endif /* _LINEAR_BOUNDS_COMPUTER_HXX_ */
