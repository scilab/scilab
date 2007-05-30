/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_JOGL_H_
#define _DRAWABLE_FIGURE_JOGL_H_

#include <jni.h>

#include "DrawableFigureImp.h"
#include "DrawableFigure.h"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

class DrawableFigureJoGL : public DrawableObjectJoGL, public DrawableFigureImp
{

public:

  DrawableFigureJoGL( DrawableFigure * drawer ) ;

  virtual ~DrawableFigureJoGL( void ) ;

  /**
  * Tell the canvas to draw itself.
  */
  virtual void drawCanvas( void ) ;

  /**
   * Display the info message of the window
   */
  virtual void updateInfoMessage( void  ) ;

  /**
   * Create the rendering canvas for the figure if none exists.
   */
  virtual void openRenderingCanvas( int figureIndex ) ;

  /**
   * Close the rendering canvas
   */
  virtual void closeRenderingCanvas( void ) ;

} ;

}

#endif /* _DRAWABLE_FIGURE_JOGL_H_  */

