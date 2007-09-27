/*------------------------------------------------------------------------*/
/* file: DrawPolylineStrategy.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy interface for the drawPolyline algorithm               */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_POLYLINE_STRATEGY_H_
#define _DRAW_POLYLINE_STRATEGY_H_

namespace sciGraphics
{

class DrawablePolyline;

class DrawPolylineStrategy
{

public:

  DrawPolylineStrategy( DrawablePolyline * polyline ) { m_pDrawed = polyline ; }

  virtual ~DrawPolylineStrategy( void ) {m_pDrawed = NULL;}


  /**
   * Main algorithm
   */
  virtual void drawPolyline( void ) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showPolyline( void ) = 0;

protected:

  DrawablePolyline * m_pDrawed;

};

}

#endif /* _DRAW_POLYLINE_STRATEGY_H_ */
