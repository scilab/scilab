/*------------------------------------------------------------------------*/
/* file: DrawSurfaceStrategy.hxx                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy interface for the drawSurface algorithm                */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_SURFACE_STRATEGY_HXX_
#define _DRAW_SURFACE_STRATEGY_HXX_

#include "DrawableSurface.h"

namespace sciGraphics
{

class DrawSurfaceStrategy
{

public:

  DrawSurfaceStrategy( DrawableSurface * surface ) { m_pDrawed = surface ; }

  virtual ~DrawSurfaceStrategy( void ) {m_pDrawed = NULL;}


  /**
   * Main algorithm
   */
  virtual void drawSurface( void ) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showSurface( void ) = 0;

protected:

  DrawableSurface * m_pDrawed;

};

}

#endif /* _DRAW_SURFACE_STRATEGY_HXX_ */
