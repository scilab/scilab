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
  // Driver Independant Algorithms
  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) = 0 ;

  /*--------------------------------------------------------------------------------------*/
  // Driver dependant algorithms

  /**
   * Create the rendering canvas for the figure.
   */
  void openRenderingCanvas( void ) ;

  /**
  * Tell the canvas to draw itself.
  */
  void drawCanvas( void ) ;

  /**
   * Close the rendering canvas
   */
  void closeRenderingCanvas( void ) ;

  /**
   * Display the info message of the window
   */
  void updateInfoMessage( void ) ;

  /**
   * Set the background color of the figure
   */
  void setBackgroundColor( void ) ;

  /*--------------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_FIGURE_H_  */
