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

namespace sciGraphics
{

/**
 * To avoid cyclic dependances.
 */
class DrawableFigure ;

class DrawableFigureImp
{

public:

  DrawableFigureImp( DrawableFigure * drawer ) { m_pDrawer = drawer ; }

  virtual ~DrawableFigureImp( void ) {}

  /**
   * Initialize the context for drawing
   */
  virtual void initializeDrawing( void ) = 0 ;

  /**
   * Close drawing session and display image on the screen
   */
  virtual void endDrawing( void ) = 0 ;

  /**
   * Display the info message of the window
   */
  virtual void updateInfoMessage( void  ) = 0 ;

protected:

  DrawableFigure * m_pDrawer ; /**< bridge for driver dependant algorithm */

} ;

}

#endif /* _DRAWABLE_FIGURE_IMP_H_  */
