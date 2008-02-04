/*------------------------------------------------------------------------*/
/* file: GrayplotDecomposer.hxx                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy decomposing grayplots objects                          */
/*------------------------------------------------------------------------*/

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
