/*------------------------------------------------------------------------*/
/* file: LogarithmicBoundsComputer.hxx                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for computing best fitting logarithmic bounds from     */
/*        user ones.                                                      */
/*------------------------------------------------------------------------*/

#ifndef _LOGARITHMIC_BOUNDS_COMPUTER_HXX_
#define _LOGARITHMIC_BOUNDS_COMPUTER_HXX_

#include "ComputeBoundsStrategy.hxx"

namespace sciGraphics
{

class LogarithmicBoundsComputer : public ComputeBoundsStrategy
{
public:

  LogarithmicBoundsComputer(void) : ComputeBoundsStrategy() {}

  virtual ~LogarithmicBoundsComputer(void) {}

  /**
  * Modify scale for (to use with non linear scale).
  */
  virtual void applyScaleModification(const double inputBounds[2], double outputBounds[2]);

  virtual void applyBestFitting(const double inputBounds[2], double outputBounds[2]);


  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double coord, double * scaledCoord);

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

#endif /* _LOGARITHMIC_BOUNDS_COMPUTER_HXX_ */
