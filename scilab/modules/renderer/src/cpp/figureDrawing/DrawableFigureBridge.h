/*------------------------------------------------------------------------*/
/* file: DrawableFigureBridge.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object                                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_BRIDGE_H_
#define _DRAWABLE_FIGURE_BRIDGE_H_

#include "../DrawableObject.h"
#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

class DrawableFigure;

class DrawableFigureBridge : public virtual DrawableObjectBridge
{

public:

  DrawableFigureBridge( void ) {}

  virtual ~DrawableFigureBridge( void ) {}

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
  virtual void setFigureParameters(void) = 0 ;

  /**
   * Draw the background of the figure
   */
  virtual void drawBackground(void) = 0;

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
   * Set pixmap mode to on or off
   */
  virtual void setPixmapMode(bool onOrOff) = 0;

  /**
   * Get the pixmap status (on or off).
   */
  virtual bool getPixmapMode(void) = 0;

  /**
   * Specify if the figure can be render or not.
   */
  virtual void setRenderingEnable(bool isEnable) = 0;

  /**
   * Return the drawed object
   */
  virtual DrawableFigure * getFigureDrawer( void ) = 0 ;

protected:

} ;

}

#endif /* _DRAWABLE_FIGURE_BRIDGE_H_  */
