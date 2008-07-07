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

#ifndef _SUBWIN_AXIS_POSITIONER_HXX_
#define _SUBWIN_AXIS_POSITIONER_HXX_

#include "DrawableSubwin.h"
#include "AxisPositioner.hxx"

namespace sciGraphics
{

/**
 * Class containing routines useful for computing axes positions along axes box
 */
class SubwinAxisPositioner : public virtual AxisPositioner
{
public:

  SubwinAxisPositioner(DrawableSubwin * subwin);

  virtual ~SubwinAxisPositioner(void);

  /**
   * Compute the direction and length of ticks
   */
  virtual void getTicksDirection(double ticksDir[3]);

protected:
  
  /*----------------------------------------------------------------------*/
  /**
   * Find the height of the bottom face
   * @return Z coordinate of the segment to draw
   */
  double findLowerZCoordinate(void);

  /**
   * Find the height of the bottom face
   * @return Z coordinate of the segment to draw
   */
  double findUpperZCoordinate(void);
  
  /**
   * Knowing on of the X bounds (xMin or xMax), find the other
   * @param xBound xMin or xMax
   * @return the other bound
   */
  double findOtherXBound(double xBound);

  /**
   * Knowing on of the y bounds (yMin or yMax), find the other
   * @param yBound yMin or yMax
   * @return the other bound
   */
  double findOtherYBound(double yBound);

  /**
   * Knowing on of the Z bounds (zMin or zMax), find the other
   * @param zBound zMin or zMax
   * @return the other bound
   */
  double findOtherZBound(double zBound);

  /**
   * Compute the X coordinate of the Y axis segment
   * Select the one wich is in front of the camera
   * @param zCoordinate zCoordinate Z coordinate of the Y axis segment
   * @return X coordinate of the segment to draw
   */
  double findFrontXCoordinate(double zCoordinate);

  /**
   * Compute the Y coordinate of the X axis segment
   * Select the one wich is in front of the camera
   * @param zCoordinate Z coordinate of the X axis segment already computed by findZCoordinate
   * @return Y coordinate of the segment to draw
   */
  double findFrontYCoordinate(double zCoordinate);

  /**
   * Compute the Y coordinate of the X axis segment
   * Select the one which on the back of the screen
   * @param zCoordinate Z coordinate of the X axis segment alredy computed by findZCoordinate
   * @return Y coordinate of the segment to draw
   */
  double findBackYCoordinate(double zCoordinate);

  /**
   * Compute the X coordinate of the Y axis segment
   * Select the one wich is on the back of the axes box
   * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
   * @return X coordinate of the segment to draw
   */
  double findBackXCoordinate(double zCoordinate);

  /**
   * Check that ticks direction is not too close to
   * the axis segemment, when projected on the screen.
   */
  bool checkTicksDirection(const double ticksDir[3],
                           const double axisSegmentStart[3],
                           const double axisSegmentEnd[3]);

  /**
   * Compute the default direction for ticks
   * Aligned with axis plane
   */
  virtual void getDefaultTicksDirection(double ticksDir[3],
                                        const double axisSegmentStart[3],
                                        const double axisSegmentEnd[3]) = 0;

  /**
   * If default direction fails the test, use this one instead
   */
  virtual void getCorrectedTicksDirection(double ticksDir[3],
                                        const double axisSegmentStart[3],
                                        const double axisSegmentEnd[3]) = 0;

  /**
   * Update bounds from the subwindow
   */
  void updateBounds(void);
  /*----------------------------------------------------------------------*/
  DrawableSubwin * m_pSubwin;

  double m_dXmin;
  double m_dXmax;
  double m_dYmin;
  double m_dYmax;
  double m_dZmin;
  double m_dZmax;
  /*----------------------------------------------------------------------*/

};

}

#endif
