/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Class containing the driver dependant routines to draw a figure object    
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
  * @return indicates if the size could be successfully modified
  */
  virtual int setSize( const int size[2] ) = 0 ;

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
   * Set the auto_resize status (on or off).
   */
  virtual void setAutoResizeMode(bool onOrOff) = 0;

  /**
   * Get tje auto_resize status (on or off).
  */
  virtual bool getAutoResizeMode(void) = 0;

  /**
   * Get the viewport on the canvas
   */
  virtual void getViewport(int viewport[4]) = 0;

  /**
   * Set the viewport on the canvas
   */
  virtual void setViewport(const int viewport[4]) = 0;

  /**
   * Set the background of the figure
   */
  virtual void setBackgroundColor(int backColor) = 0;

  /**
   * Call rubber box on the figure
   * @param isClick specify whether the rubber box is selected by mouse click
   *        or a sequence of press/release.
   * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
   * @param intialRect if not null, specify the initial rectangle to use
   * @param[out] endRect array containing the coordinates of two opposite corners of
   *                     the rubber box
   * @param[out] usedButton Scilab code of the button used to terminate the rubber box
   */
  virtual void rubberBox(bool isClick, bool isZoom, const int initialRect[4], int endRect[4], int * usedButton) = 0;

  /**
   * Set the title of the window
   */
  virtual void setTitle( const char * title ) = 0 ;

  /**
   * Get current displacement in the graphic window, to be used for axes rotation.
   * @param displacement [dx, dy] is the mouse displacement in pixels
   *         or the position of the mouse with the first call.
   * @return false if the displacement tracking has ended, true otherwise.
   */
  virtual bool getRotationDisplacement(int displacement[2]) = 0;

  /**
   * If a rotation displacement is recording, cancel it.
   */
  virtual void stopRotationRecording(void) = 0;

  /**
   * Put the figure in top of other windows.
   */
  virtual void showWindow(void) = 0;

  /**
   * Return the drawed object
   */
  virtual DrawableFigure * getFigureDrawer( void ) = 0 ;

protected:

} ;

}

#endif /* _DRAWABLE_FIGURE_BRIDGE_H_  */
