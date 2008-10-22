/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy decomposing segs and chmap object in a set of segment 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DECOMPOSE_SEGS_STRATEGY_HXX_
#define _DECOMPOSE_SEGS_STRATEGY_HXX_

#include "DrawableSegs.h"

namespace sciGraphics
{
class DecomposeSegsStrategy
{
public:

  DecomposeSegsStrategy(DrawableSegs * segs);

  virtual ~DecomposeSegsStrategy(void);

  /**
   * Compute the postions of the arraows to display
   */
  virtual void getSegsPos(double startXcoords[], double endXCoords[],
                          double startYCoords[], double endYCoords[],
                          double startZCoords[], double endZcoords[]) = 0;

  /**
   * Get the number of arrows in the segs or champ object
   */
  virtual int getNbSegment(void) = 0;

  /**
   * To know if each segs object has a distinct color
   */
  virtual bool isColored(void) = 0;

  /**
   * Get the color of each segement if needed
   */
  virtual void getSegsColors(int colors[]) = 0;

  /**
   * Get the decomposed object
   */
  DrawableSegs * getDrawedSegs(void) {return m_pDrawed;}

  /**
   * Compute the bounding box a segs object
   * Used to set the subwin size accordingly
   * @param bounds [xmin, xmax, ymin, ymax, zmin, zmax]
   */
  virtual void getBoundingBox(double bounds[6]);

protected:

  /**
   * To be used in the min and max finding
   */
  void updateMinMax(double & curMin, double & curMax, double newVal);

  DrawableSegs * m_pDrawed;

};

}

#endif /* _DECOMPOSE_SEGS_STRATEGY_HXX_ */

