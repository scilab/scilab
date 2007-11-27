/*------------------------------------------------------------------------*/
/* file: ComputeBoundsStrategy.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy template for computing real bounds from user ones.     */
/*------------------------------------------------------------------------*/

#ifndef _COMPUTE_BOUNDS_STRATEGY_HXX_
#define _COMPUTE_BOUNDS_STRATEGY_HXX_

namespace sciGraphics
{
class ComputeBoundsStrategy
{
public:

  ComputeBoundsStrategy(void) {}

  virtual ~ComputeBoundsStrategy(void) {}

  /**
   * Modify scale for (to use with non linear scale).
   */
  virtual void applyScaleModification(const double inputBounds[2], double outputBounds[2]) = 0;

  virtual void applyBestFitting(const double inputBounds[2], double outputBounds[2]) = 0;

  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double coord, double * scaledCoord) = 0;

  /**
   * Apply scale on a vector.
   */
  virtual void pointScale(double vector[], int vectorLength) = 0;

  /**
   * Apply non linear scaling on direction vector (opposite as point vector).
   * Since scaling is non linear, we need to have a starting point of the vector.
   */
  virtual void directionScale(double coord,  double startingPoint, double * scaledCoord) = 0;

  /**
   * Apply distance scale on a vector.
   */
  virtual void directionScale(double vector[], double startingPoints[], double vectorLength) = 0;

protected:

};
}

#endif /* _COMPUTE_BOUNDS_STRATEGY_HXX_ */
