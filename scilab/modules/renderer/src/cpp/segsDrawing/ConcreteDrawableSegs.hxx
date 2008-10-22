/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing implementation of driver independent 
 * algorithms of drawable segs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_SEGS_HXX_ 
#define _CONCRETE_DRAWABLE_SEGS_HXX_

#include <list>
#include "DrawableSegs.h"
#include "DrawSegsStrategy.hxx"
#include "DecomposeSegsStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableSegs : public DrawableSegs
{
public:

  ConcreteDrawableSegs(sciPointObj * pSegs);

  ~ConcreteDrawableSegs(void);

  /**
   * Get the color of each segement if needed
   */
  virtual void getSegsColors(int colors[]);

  virtual void setDecomposeStrategy(DecomposeSegsStrategy * decomposer);

  virtual void addDrawingStrategy(DrawSegsStrategy * drawer);

  virtual void removeDrawingStrategies(void);

  /**
   * Compute the bounding box a segs object
   * Used to set the subwin size accordingly
   * @param bounds [xmin, xmax, ymin, ymax, zmin, zmax]
   */
  virtual void getBoundingBox(double bounds[6]);

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Actually draw the segs
   */
  virtual EDisplayStatus drawSegs(void);

  /**
   * Draw the segs from computed positions and colors
   */
  virtual void drawSegs(const double xStarts[], const double xEnds[],
                        const double yStarts[], const double yEnds[],
                        const double zStarts[], const double zEnds[],
                        const int colors[], int nbSegment);

  /**
   * Show the segs
   */
  virtual void showSegs(void);

  /**
   * Redraw the segs
   */
  virtual void redrawSegs(void);

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
  /*---------------------------------------------------------------------------------*/
  std::list<DrawSegsStrategy *> m_oDrawers;
  DecomposeSegsStrategy * m_pDecomposer;
  /*---------------------------------------------------------------------------------*/
  
};

}

#endif /* _CONCRETE_DRAWABLE_SEGS_HXX_ */

