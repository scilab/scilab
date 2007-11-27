/*------------------------------------------------------------------------*/
/* file: DrawUIMenuStrategy.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Strategy interface for the drawUIMenu algorithm              */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_UIMENU_STRATEGY_H_
#define _DRAW_UIMENU_STRATEGY_H_

namespace sciGraphics
{
// cross reference
class DrawableUIMenuBridge ;

class DrawUIMenuStrategy
{
public:

  DrawUIMenuStrategy( DrawableUIMenuBridge * drawer ) { m_pDrawed = drawer ; }
  
  virtual ~DrawUIMenuStrategy( void ) {}

  /**
   * Main algorithm
   */
  virtual void drawUIMenu( void ) = 0;

  /**
   * Call the display list of an object
   */
  virtual void show( void ) = 0;

protected:

  DrawableUIMenuBridge * m_pDrawed ;

};
  

}

#endif /* _DRAW_UIMENU_STRATEGY_H_ */
