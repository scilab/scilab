/*------------------------------------------------------------------------*/
/* file: DrawSegsStrategy.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy interface for the drawSegs algorithm                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_SEGS_STRATEGY_HXX_
#define _DRAW_SEGS_STRATEGY_HXX_

#include "DrawableSegs.h"

namespace sciGraphics
{

class DrawSegsStrategy
{

public:

  DrawSegsStrategy( DrawableSegs * polyline ) { m_pDrawed = polyline ; }

  virtual ~DrawSegsStrategy( void ) {m_pDrawed = NULL;}


  /**
   * Main algorithm, draw the segs from their positions
   */
  virtual void drawSegs(const double xStarts[], const double xEnds[],
                        const double yStarts[], const double yEnds[],
                        const double zStarts[], const double zEnds[],
                        const int colors[], int nbSegment) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showSegs( void ) = 0;

protected:

  DrawableSegs * m_pDrawed;

};

}

#endif /* _DRAW_SEGS_STRATEGY_HXX_ */
