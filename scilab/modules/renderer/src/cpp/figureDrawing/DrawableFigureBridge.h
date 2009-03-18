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

#include "DrawableObject.h"
#include "DrawableObjectBridge.h"

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
  virtual void createVisualFigure( int figureIndex ) = 0 ;

  /**
   * Close the rendering canvas
   */
  virtual void closeVisualFigure( void ) = 0 ;

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
	 * Get the info message of the figure.
	 * @param[out] message string filled with the message. Should have a minimum size
	 *                     given by getInfoMessageLength.
	 */
	virtual void getInfoMessage(char * message) = 0;

	/**
	 * @return size of the info message to be used in combination with getInfoMessage.
	 */
	virtual int getInfoMessageLength(void) = 0;

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
   * @param intialRect if not null, specify the initial rectangle to use
   * @param[out] endRect array containing the coordinates of two opposite corners of
   *                     the rubber box
   * @param[out] usedButton Scilab code of the button used to terminate the rubber box
   */
  virtual void rubberBox(bool isClick, const int initialRect[4], int endRect[4], int * usedButton) = 0;

	/**
	 * Perfomr an interactive zoom of the figure using a rectangular selection.
	 */
	virtual void interactiveZoom(void) = 0;

  /**
   * Set the title of the window
   */
  virtual void setTitle( const char * title ) = 0 ;

	/**
	 * Perform an interactive rotation of a subwin contained in the figure.
	 */
	virtual void interactiveRotation(void) = 0;

  /**
   * Put the figure in top of other windows.
   */
  virtual void showWindow(void) = 0;

	/**
	 * Create a graphic canvas for the figure.
	 * Figure is then able to draw 3d plots.
	 */
	virtual void openGraphicCanvas(void) = 0;

	/**
	 * Remove the graphic canvas for the figure.
	 * Figure is no longer able to display 3D plots.
	 */
	virtual void closeGraphicCanvas(void) = 0;

	/**
	 * Force the window to be be single buffered
	 */
	virtual void setUseSingleBuffer(bool useSingleBuffer) = 0;


	/**
	 * @return the number of pass used for antialiasing or 0 if antialiasing is disable.
	 */
	virtual int getAntialiasingQuality(void) = 0;
	
	/**
	 * Modify the quality of antialiasing or disable it.
	 * If quality if 0, the antialiasing is disables,
	 * otherwise it might be either 1, 2, 4, 8 or 16 and then
	 * specify the number of pass for antialiasing.
	 * @param quality positive integer.
	 */
	virtual void setAntialiasingQuality(int quality) = 0;

  /**
   * Return the drawed object
   */
  virtual DrawableFigure * getFigureDrawer( void ) = 0 ;

protected:

} ;

}

#endif /* _DRAWABLE_FIGURE_BRIDGE_H_  */
