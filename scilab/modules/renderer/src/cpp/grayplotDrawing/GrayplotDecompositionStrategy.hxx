/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for converting grayplots into a grid and color
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAYPLOT_DECOMPOSITION_STRATEGY_HXX_
#define _GRAYPLOT_DECOMPOSITION_STRATEGY_HXX_

#include "DrawableGrayplot.h"

namespace sciGraphics
{

class GrayplotDecompositionStrategy
{
public:

  GrayplotDecompositionStrategy(DrawableGrayplot * grayplot) {m_pDrawed = grayplot;}

  virtual ~GrayplotDecompositionStrategy(void) {m_pDrawed = NULL;}

  /**
   * Get the grid size along X axis
   */
  virtual int getNbRow(void) = 0;

  /**
   * Get the grid size along Y axis
   */
  virtual int getNbCol(void) = 0;

  /**
   * Get the size of the colors, or number of facets
   */
  virtual int getNbColors(void) = 0;

  /**
   * Get the constant Z coordinate to use for the grayplot
   */
  virtual double getZCoordinate(void) = 0;

  /**
   * COmpute positions of the grid and the color of each facet
   */
  virtual void decomposeGrayplot(double xGrid[], double yGrid[], int colors[]) = 0;

protected:

  DrawableGrayplot * m_pDrawed;

};

}

#endif /* _GRAYPLOT_DECOMPOSITION_STRATEGY_HXX_ */

