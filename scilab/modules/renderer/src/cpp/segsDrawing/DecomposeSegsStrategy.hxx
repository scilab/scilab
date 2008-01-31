/*------------------------------------------------------------------------*/
/* file: DecomposeSegsStrategy.hxx                                        */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy decomposing segs and chmap object in a set of segment  */
/*------------------------------------------------------------------------*/

#ifndef _DECOMPOSE_SEGS_STRATEGY_HXX_
#define _DECOMPOSE_SEGS_STRATEGY_HXX_

#include "DrawableSegs.h"

namespace sciGraphics
{
class DecomposeSegsStrategy
{
public:

  DecomposeSegsStrategy(DrawableSegs * segs) {m_pDrawed = segs;}

  virtual ~DecomposeSegsStrategy(void) {m_pDrawed = NULL;}

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

protected:

  DrawableSegs * m_pDrawed;

};

}

#endif /* _DECOMPOSE_SEGS_STRATEGY_HXX_ */
