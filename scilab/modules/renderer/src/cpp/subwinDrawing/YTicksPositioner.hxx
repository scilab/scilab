/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Y_TICKS_POSITIONER_HXX_
#define _Y_TICKS_POSITIONER_HXX_

#include "PlaceTicksStrategy.hxx"

namespace sciGraphics
{

/**
 * Class specialized in positioning ticks along X axis
 */
class YTicksPositioner : public PlaceTicksStrategy
{
public:

  YTicksPositioner(DrawableSubwin * subwin);

  virtual ~YTicksPositioner(void);

protected:

  /**
   * Compute the two edges of axis segment in 3D coordinates
   */
  virtual void computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3]);

  /**
   * Compute direction to be used by ticks in 3D coordinates
   */
  virtual void computeTicksDir3D(double ticksDir[3]);

  /**
   * Convert ticks abscissas along axis into positions in 3D coordinates
   * Also remove out of bounds legends and ticks.
   * @param nbTicksIn number of ticks position in ticksAbscissas
   * @param[out] nbTicksOut number of ticks in final ticks positions
   */
  virtual void computeTicksLocation3D(const double ticksAbscissas[],
                                      double ticksXPos[],
                                      double ticksYPos[],
                                      double ticksZPos[],
                                      char * labels[],
                                      char * labelsExponents[],
                                      int nbTicksIn,
                                      int * nbTicksOut);

  /**
   * Compute the two edges of axis segment in 3D coordinates
   * knowing x and z coordinate of X axis
   */
  void computeAxisBounds3D(double axisStart[3], double axisEnd[3], double xCoordinate, double zCoordinate);

  /**
   * Compute direction to be used by ticks in 3D coordinates
   * knowing x and z coordinate of X axis
   */
  void computeTicksDir3D(double ticksDir[3], double xCoordinate, double zCoordinate);

  /**
   * Convert ticks abscissas along axis into positions in 3D coordinates
   * knowing x and z coordinate of X axis
   * Also remove out of bounds legends and ticks.
   * @param nbTicksIn number of ticks position in ticksAbscissas
   * @param[out] nbTicksOut number of ticks in final ticks positions
   */
  void computeTicksLocation3D(const double ticksAbscissas[],
                              double ticksXPos[],
                              double ticksYPos[],
                              double ticksZPos[],
                              char * labels[],
                              char * labelsExponents[],
                              int nbTicksIn,
                              int * nbTicksOut,
                              double xCoordinate,
                              double zCoordinate);

  /**
   * Compute the start points to use for grid drawing in 3D coordinates
   * To be redifined by ticks which also draw grid
   */
  virtual void computeGridStartPoints3D(const double ticksAbscissas[],
                                         const double subticksAbscissas[],
                                         double startXPos[],
                                         double startYPos[],
                                         double startZPos[],
                                         int nbTicksIn,
                                         int nbSubticksIn,
                                         int * nbTicksOut);

  /**
   * Compute the middle points to use for grid drawing in 3D coordinates
   * To be redifined by ticks which also draw grid
   */
  virtual void computeGridMiddlePoints3D(const double ticksAbscissas[],
                                         const double subticksAbscissas[],
                                         double middleXPos[],
                                         double middleYPos[],
                                         double middleZPos[],
                                         int nbTicksIn,
                                         int nbSubticksIn,
                                         int * nbTicksOut);

  /**
   * Compute the middle points to use for grid drawing in 3D coordinates
   * To be redifined by ticks which also draw grid
   */
  virtual void computeGridEndPoints3D(const double ticksAbscissas[],
                                      const double subticksAbscissas[],
                                      double endXPos[],
                                      double endYPos[],
                                      double endZPos[],
                                      int nbTicksIn,
                                      int nbSubticksIn,
                                      int * nbTicksOut);

  /**
  * Compute the default direction of ticks on Y axis, parallel to X axis
  * @param yCoordinate X coordinate of the Y axis
  * @param zCoordinate Z coordinate of the Y axis
  * @param ticksDirection of the ticks normalized
  */
  void findYTicksDirection(double ticksDirection[3], double xCoordinate, double zCoordinate);

  /**
   * Find the Z coordinate of the Y axis segment.
   * @return Z coordinate of the segment to draw
   */
  virtual double findZCoordinate(void) = 0;
	
  /**
   * Compute the X coordinate of the Y axis segment
   * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
   * @return X coordinate of the segment to draw
   */
  virtual double findXCoordinate(double zCoordinate) = 0;


};

}


#endif /* _Y_TICKS_POSITIONER_HXX_ */
