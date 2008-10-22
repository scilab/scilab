/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for computing best fitting logarithmic bounds from user ones. 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
   * Apply inverse scale on a vector.
   */
  virtual void inversePointScale(double vector[], int vectorLength);

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

