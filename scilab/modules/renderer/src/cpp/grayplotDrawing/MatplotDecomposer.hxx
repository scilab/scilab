/*------------------------------------------------------------------------*/
/* file: MatplotDecomposer.hxx                                            */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy decomposing matplot objects                            */
/*------------------------------------------------------------------------*/

#ifndef _MATPLOT_DECOMPOSER_HXX_ 
#define _MATPLOT_DECOMPOSER_HXX_

#include "GrayplotDecompositionStrategy.hxx"

namespace sciGraphics
{

class MatplotDecomposer : public GrayplotDecompositionStrategy
{
public:

  MatplotDecomposer(DrawableGrayplot * grayplot);

  virtual ~MatplotDecomposer(void);

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
   * Compute position of the grid in matplot mode
   * this means bounds are (1,nx) x (1,ny) 
   */
  void decomposeMatplot(double xGrid[], double yGrid[]);

  /**
   * Compute position of the grid in matplot1 mode
   * this mean that bounds are specified
   */
  void decomposeMatplot1(double xGrid[], double yGrid[]);

};

}

#endif /* _GRAYPLOT_DECOMPOSER_HXX_ */
