/*------------------------------------------------------------------------*/
/* file: DrawRectangleStrategy.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy interface for the drawRectangle algorithm              */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_RECTANGLE_STRATEGY_H_
#define _DRAW_RECTANGLE_STRATEGY_H_

namespace sciGraphics
{
  // cross reference
  class DrawableRectangleImp ;

  class DrawRectangleStrategy
  {
  public:

    DrawRectangleStrategy( DrawableRectangleImp * drawer ) { m_pDrawed = drawer ; }
    
    virtual ~DrawRectangleStrategy( void ) {}

    /**
     * Main algorithm
     */
    virtual void drawRectangle( void ) = 0 ;

  protected:

    DrawableRectangleImp * m_pDrawed ;

  };
  

}

#endif /* _DRAW_RECTANGLE_STRATEGY_H_ */
