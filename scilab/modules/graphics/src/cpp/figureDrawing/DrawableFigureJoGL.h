/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_JOGL_H_
#define _DRAWABLE_FIGURE_JOGL_H_


#include "DrawableFigureImp.h"
#include "DrawableFigure.h"

namespace sciGraphics
{

class DrawableFigureJoGL : public DrawableFigureImp
{

public:

  DrawableFigureJoGL( DrawableFigure * drawer ) : DrawableFigureImp( drawer ) {}

  /**
   * Initialize the context for drawing
   */
  virtual void initializeDrawing( void ) ;

  /**
   * Close drawing session and display image on the screen
   */
  virtual void endDrawing( void ) ;

  /**
   * Display the info message of the window
   */
  virtual void updateInfoMessage( void  ) ;

} ;

}

#endif /* _DRAWABLE_FIGURE_JOGL_H_  */
