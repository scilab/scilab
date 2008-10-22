/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Z_AXIS_POSITIONER_HXX_
#define _Z_AXIS_POSITIONER_HXX_

#include "SubwinAxisPositioner.hxx"

namespace sciGraphics
{

/**
 * Interface for determining on which edge of axes box an axe will be
 */
class ZAxisPositioner : public SubwinAxisPositioner
{
public:

  ZAxisPositioner(DrawableSubwin * subwin);

  virtual ~ZAxisPositioner(void);

  /**
   * Get the two bounds of this axis.
   */
  virtual void getAxisBounds(double startBound[3], double endBound[3]);

  /**
   * Find the edges on which the grid will be displayed from the axes.
   * Box. Grid is drawn on 2 facets, then 3 edges are necessary.
   * This function is not useful for all axes drawer and drawer
   * which need it must redefine it.
   */
  virtual void getGridEdges(double startBound1[3], double startBound2[3],
                            double middleBound1[3], double middleBound2[3],
                            double endBound1[3], double endBound2[3]);

  /**
   * Convert ticks abscissas, into relative coordinates
   * between the two axis bounds
   * @param ticksPos abscissas of ticks along their axis as input
   *                 in output, get the relative coordinates between the axes bounds.
   *                 0 stands fot the start bound, 1 for the last one.
   * @param nbTicks number of ticks abascissas as input
   * @return number of ticks as output
   */
  virtual int getRelativeTicksPosition(double ticksPos[], int nbTicks);

protected:

  /**
   * Find the X coordinate of the Z axis segment.
   * Z axis segment is always displayed on the left of the box.
   * @return X coordinate of the segment to draw
   */
  double findLeftMostXCoordinate(void);

  /**
   * Compute the Y coordinate of the Z axis segment
   * @return Y coordinate of the segment to draw
   */
  double findLeftMostYCoordinate(void);

  /**
   * To know if the Z axis is sharing an edge with X bottom axis
   * @param zCoordFront Z coordinate of the front point
   * @param xCoordinate X coordinate of the Z axis
   * @return true if X axis and Z axis has a common edge
   */
  bool isSharingEndWithXaxis(double zCoordFront, double xCoordinate);

  /**
   * Compute the default direction for ticks
   * Aligned with axis plane
   */
  virtual void getDefaultTicksDirection(double ticksDir[3],
                                        const double axisSegmentStart[3],
                                        const double axisSegmentEnd[3]);

  /**
   * If default direction fails the test, use this one instead
   */
  virtual void getCorrectedTicksDirection(double ticksDir[3],
                                        const double axisSegmentStart[3],
                                        const double axisSegmentEnd[3]);

};

}

#endif
