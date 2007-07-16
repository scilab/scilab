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

class DrawableFigure;

class DrawableFigureImp : public virtual DrawableObjectImp
{

public:

  DrawableFigureImp( void ) {}

  virtual ~DrawableFigureImp( void ) {}

  /**
  * Tell the canvas to draw itself.
  */
  virtual void drawCanvas( void ) = 0 ;

  /**
   * Create the rendering canvas for the figure.
   */
  virtual void openRenderingCanvas( int figureIndex ) = 0 ;

  /**
   * Close the rendering canvas
   */
  virtual void closeRenderingCanvas( void ) = 0 ;

  /**
   * Set the background color of the figure
   */
  virtual void setBackgroundColor( int backgroundColor ) = 0 ;

  /**
   * Set a new colormap to this figure
   * @param nbColor x 3 matrix contains the three channels RGB of each color
   * @param nbColor number of color in the colormap
   */
  virtual void setColorMap( const double rgbMat[], int nbColor ) = 0 ;

  /**
   * Fill the rgbMat with the colors in the colormap
   * @param rgbMat should be a nbColor x 3 matrix with nbColor the number of color in the colormap
   */
  virtual void getColorMap( double rgbMat[] ) = 0 ;

  /**
  * Get the size (width, height) of the figure in parameter size.
  */
  virtual void getSize( int size[2] ) = 0 ;

  /**
  * Set the size (width, height) of the figure.
  */
  virtual void setSize( const int size[2] ) = 0 ;

  /**
  * Get the size of the window enclosing the figure.
  */
  virtual void getWindowSize( int size[2] ) = 0 ;

  /**
  * Set the size of the window enclosing the figure.
  */
  virtual void setWindowSize( const int size[2] ) = 0 ;

  /**
   * Get the size of the window enclosing the figure.
   */
  virtual void getWindowPosition( int pos[2] ) = 0 ;

  /**
   * Set the size of the window enclosing the figure.
   */
  virtual void setWindowPosition( const int pos[2] ) = 0 ;

  /**
  * Set the displayed the info message of the window
  */
  virtual void setInfoMessage( const char * infoMessage ) = 0 ;

  /**
   * Return the drawed object
   */
  virtual DrawableFigure * getFigureDrawer( void ) = 0 ;

protected:

} ;

}

#endif /* _DRAWABLE_FIGURE_IMP_H_  */
