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
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

class DrawableFigureJoGL : public DrawableFigureImp, public DrawableObjectJoGL
{

public:

  DrawableFigureJoGL( DrawableFigure * drawer ) ;

  virtual ~DrawableFigureJoGL( void ) ;

  /**
  * Tell the canvas to draw itself.
  */
  virtual void drawCanvas( void ) ;

  /**
   * Create the rendering canvas for the figure if none exists.
   */
  virtual void openRenderingCanvas( int figureIndex ) ;

  /**
   * Close the rendering canvas
   */
  virtual void closeRenderingCanvas( void ) ;

  /**
   * Set the background color of the figure
   */
  virtual void setBackgroundColor( int backgroundColor ) ;

  /**
   * Set a new colormap to this figure
   * @param nbColor x 3 matrix contains the three channels RGB of each color
   * @param nbColor number of color in the colormap
   */
  virtual void setColorMap( const double rgbMat[], int nbColor ) ;

  /**
   * Fill the rgbMat with the colors in the colormap
   * @param rgbMat should be a nbColor x 3 matrix with nbColor the number of color in the colormap
   */
  virtual void getColorMap( double rgbMat[] ) ;

  /**
   * Get the size (width, height) of the figure in parameter size.
   */
  virtual void getSize( int size[2] ) ;

  /**
   * Set the size (width, height) of the figure.
   */
  virtual void setSize( const int size[2] ) ;

  /**
   * Get the size of the window enclosing the figure.
   */
  virtual void getWindowSize( int size[2] ) ;

  /**
   * Set the size of the window enclosing the figure.
   */
  virtual void setWindowSize( const int size[2] ) ;

  /**
   * Get the size of the window enclosing the figure.
   */
  virtual void getWindowPosition( int pos[2] ) ;

  /**
   * Set the size of the window enclosing the figure.
   */
  virtual void setWindowPosition( const int pos[2] ) ;

  /**
   * Set the message displayed by the figure.
   */
  virtual void setInfoMessage( const char * message ) ;

} ;

}

#endif /* _DRAWABLE_FIGURE_JOGL_H_  */

