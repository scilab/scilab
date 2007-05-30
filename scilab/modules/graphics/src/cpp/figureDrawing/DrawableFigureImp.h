/*------------------------------------------------------------------------*/
/* file: DrawableFigureImp.h                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object                                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_IMP_H_
#define _DRAWABLE_FIGURE_IMP_H_

#include "../DrawableObject.h"
#include "../DrawableObjectImp.h"

namespace sciGraphics
{

/**
 * To avoid cyclic dependancies.
 */
class DrawableFigure ;

class DrawableFigureImp : virtual public DrawableObjectImp
{

public:

  DrawableFigureImp( DrawableFigure * drawer ) { m_pDrawer = drawer ; }

  virtual ~DrawableFigureImp( void ) {}

  /**
  * Tell the canvas to draw itself.
  */
  virtual void drawCanvas( void ) = 0 ;

  /**
   * Display the info message of the window
   */
  virtual void updateInfoMessage( void  ) = 0 ;

  /**
   * Create the rendering canvas for the figure.
   */
  virtual void openRenderingCanvas( int figureIndex ) = 0 ;

  /**
   * Close the rendering canvas
   */
  virtual void closeRenderingCanvas( void ) = 0 ;

protected:

  DrawableFigure * m_pDrawer ; /**< bridge for driver dependent algorithm */

} ;

}

#endif /* _DRAWABLE_FIGURE_IMP_H_  */
