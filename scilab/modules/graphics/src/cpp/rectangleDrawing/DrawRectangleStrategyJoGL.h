/*------------------------------------------------------------------------*/
/* file: DrawRectangleStrategyJoGL.h                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy interface for the drawRectangle algorithm              */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_RECTANGLE_STRATEGY_JOGL_H_
#define _DRAW_RECTANGLE_STRATEGY_JOGL_H_

namespace sciGraphics
{
  // cross reference
  class DrawableRectangleJoGL ;

  class DrawRectangleStrategyJoGL
  {
  public:

    DrawRectangleStrategyJoGL( DrawableRectangleJoGL * drawer ) { m_pDrawer = drawer ; }
    
    virtual ~DrawRectangleStrategyJoGL( void ) {}

    /**
     * Main algorithm
     */
    virtual void drawRectangle( void ) = 0 ;

  protected:

    DrawableRectangleJoGL * m_pDrawer ;

  };
  

}

#endif /* _DRAW_RECTANGLE_STRATEGY_JOGL_H_ */
