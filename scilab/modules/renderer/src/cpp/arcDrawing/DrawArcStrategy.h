/*------------------------------------------------------------------------*/
/* file: DrawArcStrategy.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy interface for the drawArc algorithm                    */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_ARC_STRATEGY_H_
#define _DRAW_ARC_STRATEGY_H_

namespace sciGraphics
{

class DrawableArc;

class DrawArcStrategy
{

public:

  DrawArcStrategy( DrawableArc * arc ) { m_pDrawed = arc ; }
  
  virtual ~DrawArcStrategy( void ) {}


  /**
   * Main algorithm
   */
  virtual void drawArc( void ) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showArc( void ) = 0;

protected:

  DrawableArc * m_pDrawed;

};

}

#endif /* _DRAW_ARC_STRATEGY_H_ */
