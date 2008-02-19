/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy decomposing grayplots objects
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAYPLOT_DECOMPOSER_HXX_ 
#define _GRAYPLOT_DECOMPOSER_HXX_

#include "GrayplotDecompositionStrategy.hxx"

namespace sciGraphics
{

class GrayplotDecomposer : public GrayplotDecompositionStrategy
{
public:
  
  GrayplotDecomposer(DrawableGrayplot * grayplot);

  virtual ~GrayplotDecomposer(void);

  /**
   * Get the grid size along X axis
   */
  virtual int getNbRow(void);

  /**
   * Get the grid size along Y axis
   */
  virtual int getNbCol(void);

  /**
   * Get the size of the colors, or number of facets
   */
  virtual int getNbColors(void);

  /**
   * Get the constant Z coordinate to use for the grayplot
   */
  virtual double getZCoordinate(void);

  /**
   * Compute positions of the grid and the color of each facet
   */
  virtual void decomposeGrayplot(double xGrid[], double yGrid[], int colors[]);

protected:

  /**
   * Decompose color in scaled mode
   */
  void decomposeScaledColors(int colors[]);

  /**
   * Decompose color in direct mode
   */
  void decomposeDirectColors(int colors[]);

private:

  /**
   * Get the color of the facet column i, row j
   * which is the mean of the colors on the facet vertex
   */
  double getFacetZvalue(const double zValues[], int nbCol, int i, int j);

};

}

#endif /* _GRAYPLOT_DECOMPOSER_HXX_ */

