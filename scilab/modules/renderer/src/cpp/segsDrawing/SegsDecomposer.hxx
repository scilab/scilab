/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy decomposing only segs object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SEGS_DECOMPOSER_HXX_ 
#define _SEGS_DECOMPOSER_HXX_

#include "DecomposeSegsStrategy.hxx"

namespace sciGraphics
{

class SegsDecomposer : public DecomposeSegsStrategy
{
public:

  SegsDecomposer(DrawableSegs * segs);

  virtual ~SegsDecomposer(void);

  /**
   * Compute the postions of the arraows to display
   */
  virtual void getSegsPos(double startXcoords[], double endXCoords[],
                          double startYCoords[], double endYCoords[],
                          double startZCoords[], double endZcoords[]);

  /**
   * Get the number of arrows in the segs or champ object
   */
  virtual int getNbSegment(void);

  /**
   * To know if each segs object has a distinct color
   */
  virtual bool isColored(void);

  /**
   * Get the color of each segement if needed
   */
  virtual void getSegsColors(int colors[]);

protected:

};

}

#endif /* _SEGS_DECOMPOSER_HXX_ */

