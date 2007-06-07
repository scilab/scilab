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

  DrawableFigureImp( DrawableFigure * drawer ) ;

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
   * Return the drawed object
   */
  DrawableFigure * getDrawer( void ) ;

protected:

} ;

}

#endif /* _DRAWABLE_FIGURE_IMP_H_  */
