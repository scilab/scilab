/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Class containing the drawing routine for a figure object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FIGURE_H_
#define _DRAWABLE_FIGURE_H_

#include <list>

#include "DrawableObject.h"
#include "DrawableFigureBridge.h"
#include "GraphicSynchronization/GraphicSynchronizer.h"

namespace sciGraphics
{

class DrawableFigure : public DrawableObject
{

public:

  DrawableFigure( sciPointObj * pObj ) ;

  virtual ~DrawableFigure( void ) ;

  /**
   * Force display of the graphic window from the Scilab thread.
   */
  virtual void forceDisplay( void ) ;

  /**
   * Force a future redraw all subwindows contained in the figure.
   */
  void redrawSubwins( void );

  /**
   * Like display but to display only a set of objects
   */
  virtual void drawSingleObjs(std::list<sciPointObj *>& singleObjects) = 0;

  /*-----------------------------------------------------------------------------*/
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
   * @return indicates if the size could be successfully modified
   */
   int setSize( const int size[2] ) ;

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
	 * Get the info message of the figure.
	 * @param[out] message string filled with the message. Should have a minimum size
	 *                     given by getInfoMessageLength.
	 */
	void getInfoMessage(char * message);

	/**
	 * @return size of the info message to be used in combination with getInfoMessage.
	 */
	int getInfoMessageLength(void);

   /**
    * Set the auto_resize status (on or off).
    */
   void setAutoResizeMode(bool onOrOff);

   /**
    * Get tje auto_resize status (on or off).
    */
   bool getAutoResizeMode(void);

   /**
    * Get the viewport on the canvas
    */
   void getViewport(int viewport[4]);

   /**
    * Set the viewport on the canvas
    */
   void setViewport(const int viewport[4]);

   /**
    * Set the background of the figure
    */
   void setBackgroundColor(int backColor);

   /**
    * Call rubber box on the figure
    * @param isClick specify whether the rubber box is selected by mouse click
    *        or a sequence of press/release.
    * @param intialRect if not null, specify the initial rectangle to use
    * @param[out] endRect array containing the coordinates of two opposite corners of
    *                     the rubber box
    * @param[out] usedButton Scilab code of the button used to terminate the rubber box
    */
   void rubberBox(bool isClick, const int initialRect[4], int endRect[4], int * usedButton);

	 /**
	  * Perfomr an interactive zoom of the figure using a rectangular selection.
	  */
	 void interactiveZoom(void);

   /**
    * Create the rendering canvas for the figure.
    */
   void createVisualFigure( void ) ;

  /**
   * Set the figure title
   * @param[in] title the figure title
   */
   void setTitle(const char * title);

	/**
	 * Perform an interactive rotation of a subwin contained in the figure.
	 */
	void interactiveRotation(void);

  /**
   * Put the figure in top of other windows.
   */
  void showWindow(void);

	/**
	 * Create a graphic canvas for the figure.
	 * Figure is then able to draw 3d plots.
	 */
	void openGraphicCanvas(void);

	/**
	 * Remove the graphic canvas for the figure.
	 * Figure is no longer able to display 3D plots.
	 */
	void closeGraphicCanvas(void);

	/**
	 * Force the window to be be single buffered
	 */
	void setUseSingleBuffer(bool useSingleBuffer);

  /**
   * Check wether we the environemebnt allows us to create a new window.
   */
  static bool isAbleToCreateFigure(void);

  /*-----------------------------------------------------------------------------*/
   /**
    * Get the synchronization object that protect this figure.
    */
   GraphicSynchronizer * getSynchronizer( void ) { return m_pSynchronizer; }

   /**
    * Set the synchronization object that protect this figure.
    */
   void setSynchronizer( GraphicSynchronizer * synchronizer ) { m_pSynchronizer = synchronizer ; }
  /*-----------------------------------------------------------------------------*/

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Return the real type of implementation object
   */
  DrawableFigureBridge * getFigureImp( void ) ;

  
  /*-----------------------------------------------------------------------------*/
  // Driver independent Algorithms
  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) = 0 ;

  /**
   * Check wether the whole figure and hierechy should be redrawn
   * or just a single object.
   */
  virtual bool isDisplayingSingleObject(void) = 0;

  /**
   * Display the single object
   */
  virtual void displaySingleObject(void) = 0;
  /*-----------------------------------------------------------------------------*/
  // Driver dependant algorithms

  /**
   * Tell the canvas to draw itself.
   */
  void drawCanvas( void ) ;

  /**
   * Close the rendering canvas
   */
  void closeVisualFigure( void ) ;

  /**
   * Set the background color of the figure
   */
  void setFigureParameters( void ) ;

	 /**
    * Draw the background of the figure
    */
   void drawBackground(void);

  /*-----------------------------------------------------------------------------*/
  /** Synchronizer object used to protect data of the figure and its children */
  GraphicSynchronizer * m_pSynchronizer;
  /*-----------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_FIGURE_H_  */
