/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy template for computing real bounds from user ones. 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

  /**
   * Modify data bounds in order to have nice ticks labels (not 0.9545121 for example).
   */
  virtual void applyBestFitting(const double inputBounds[2], double outputBounds[2]) = 0;

  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double coord, double * scaledCoord) = 0;

  /**
   * Inverse of pointScale.
   */
  virtual void inversePointScale(double scaledCoord, double * coord) = 0;

  /**
   * Apply scale on a vector.
   */
  virtual void pointScale(double vector[], int vectorLength) = 0;

  /**
   * Apply inverse scale on a vector.
   */
  virtual void inversePointScale(double vector[], int vectorLength) = 0;

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
