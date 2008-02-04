/*------------------------------------------------------------------------*/
/* file: GrayplotDecompositionStrategy.hxx                                */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for converting grayplots into a grid and color         */
/*------------------------------------------------------------------------*/

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
