/*------------------------------------------------------------------------*/
/* file: DrawableFigure.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a figure object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_H_
#define _DRAWABLE_FIGURE_H_

#include "../DrawableObject.h"
#include "DrawableFigureImp.h"

namespace sciGraphics
{

class DrawableFigure : public DrawableObject
{

public:

  DrawableFigure( sciPointObj * pObj ) ;

  virtual ~DrawableFigure( void ) ;

  /**
   * Real operations to draw the figure.
   * Can be only called if the rendering context (canvas)
   * is created
   */
  virtual void drawInContext( void ) ;

  /*--------------------------------------------------------------------------------------*/
  /* Set/Get properties */

  /**
   * Set a new colormap to this figure
   * @param nbColor x 3 matrix contains the three channels RGB of each color
   * @param nbColor number of color in the colormap
   */
  void setColorMap( const double rgbMat[], int nbColor ) ;

  /**
   * Fill the rgbMat with the colors in the colormap
   * @param rgbMat should be a nbColor x 3 matrix with nbColor the number of color in the colormap
   */
  void getColorMap( double rgbMat[] ) ;

  /**
   * Get the size (width, height) of the figure in parameter size.
   */
  void getSize( int size[2] ) ;

  /**
   * Set the size (width, height) of the figure.
   */
   void setSize( const int size[2] ) ;

   /**
    * Get the size of the window enclosing the figure.
    */
   void getWindowSize( int size[2] ) ;

   /**
    * Set the size of the window enclosing the figure.
    */
   void setWindowSize( const int size[2] ) ;

   /**
    * Get the size of the window enclosing the figure.
    */
   void getWindowPosition( int pos[2] ) ;

   /**
    * Set the size of the window enclosing the figure.
    */
   void setWindowPosition( const int pos[2] ) ;

   /**
    * Set the displayed the info message of the window
    */
   void setInfoMessage( const char * infoMessage ) ;

   /**
    * Create the rendering canvas for the figure.
    */
   void openRenderingCanvas( void ) ;
  /*--------------------------------------------------------------------------------------*/

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) ;

  /**
   * Return the real type of implementation object
   */
  DrawableFigureImp * getFigureImp( void ) ;
  
  /*--------------------------------------------------------------------------------------*/
  // Driver independent Algorithms
  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) = 0 ;


  /*--------------------------------------------------------------------------------------*/
  // Driver dependant algorithms

  /**
  * Tell the canvas to draw itself.
  */
  void drawCanvas( void ) ;

  /**
   * Close the rendering canvas
   */
  void closeRenderingCanvas( void ) ;

  /**
   * Set the background color of the figure
   */
  void setBackgroundColor( void ) ;

  /*--------------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_FIGURE_H_  */
