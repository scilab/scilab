/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * desc : Properties needed by a figure from its rendering canvas
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.figureDrawing;

import javax.media.opengl.GL;


/**
 * Properties needed by a figure from its rendering canvas
 * @author Jean-Baptiste Silvy
 */
public interface RendererProperties {

	/** return status when resize is ok */
	int RESIZE_SUCCESS = 0;
	/** Resize failed because other docked objects are present */
	int RESIZE_MULTIPLE_DOCKED_TAB = -1;
	/** Not enough resource to allocate the new canvas  */
	int RESIZE_SIZE_TOO_LARGE = -2;
	
	/**
	 * get the GL pipeline used by the canvas
	 * @return current GL pipeline
	 */
	GL getGLPipeline();
	
	/**
	 * Get the info message used by the window
	 * @return info message
	 */
	String getInfoMessage();
	
	/**
	 * Set the info message of the figure
	 * @param infoMessage info message
	 */
	void setInfoMessage(String infoMessage);
	
	/**
	 * Force display of the rendering canvas
	 */
	void forceDisplay();
	
	/**
	 * Get the rendering canvas width
	 * @return width in pixel
	 */
	int getCanvasWidth();
	
	/**
	 * Get the rendering canvas height
	 * @return height in pixel
	 */
	int getCanvasHeight();
	
	/**
	 * Specify a new size for the rendering canvas
	 * @param width new width in pixels
	 * @param height new height in pixels
	 * @return indicates if the size could be successfully modified
	 */
	int setCanvasSize(int width, int height);
	
	/**
	 * Get the rendering window width
	 * @return width in pixel
	 */
	int getWindowWidth();
	
	/**
	 * Get the rendering window height
	 * @return height in pixel
	 */
	int getWindowHeight();
	
	/**
	 * Specify a new size for the rendering window
	 * @param width	new width in pixel
	 * @param height new height in pixel
	 */
	void setWindowSize(int width, int height);
	
	/**
	 * Get the position of the rendering window on the screen
	 * @return position along X axis in pixel
	 */
	int getWindowPosX();
	
	/**
	 * Get the position of the rendering window on the screen
	 * @return position along Y axis in pixel 
	 */
	int getWindowPosY();
	
	/**
	 * Specify a new position for the rendering window on the screen
	 * @param posX new position along X axis in pixel
	 * @param posY new position along Y axis in pixel
	 */
	void setWindowPosition(int posX, int posY);
	
	/**
     * Set the pixmap mode of the canvas to on or off.
	 * @param onOrOff specufy on or off
	 */
    void setPixmapMode(boolean onOrOff);
    
    /**
     * @return wether the pixmap mode is on or off.
     */
    boolean getPixmapMode();
    
    /**
     * Close the figure
     */
    void closeFigure();
    
    /**
     * Specify wether the canvas should fit the parent tab size
     * (and consequently the scrollpane size) or not
     * @param onOrOff true to enable autoresize mode
     */
    void setAutoResizeMode(boolean onOrOff);
    
    /**
     * @return wether the resize mode is on or off
     */
    boolean getAutoResizeMode();
    
    /**
	 * Get the part of the canvas which is currently viewed
	 * @return [x,y,w,h] array
	 */
    int[] getViewport();
    
    /**
	 * Specify a new viewport for the canvas
	 * For SwingScilabCanvas viewport can not be modified
	 * since it match the parent tab size
	 * @param posX X coordinate of upper left point of the viewport within the canvas
	 * @param posY Y coordinate of upper left point of the viewport within the canvas
	 * @param width width of the viewport
	 * @param height height of the viewport
	 */
	void setViewport(int posX, int posY, int width, int height);
	
	/**
	 * Set the background of the Canvas.
	 * @param red red channel
	 * @param green green channel 
	 * @param blue blue channel
	 */
	void setBackgroundColor(double red, double green, double blue);
	
	/**
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param figureIndex index of the figure on which the rubber box is applied
	 * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
	 *                or a sequence of press-release
	 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	int rubberBox(int figureIndex, boolean isClick, boolean isZoom, int[] initialRect, int[] endRect);
	
	/**
	 * Set the title of the figure
	 * @param title the title
	 */
	void setTitle(String title);
	
	/**
	 * Get the title of the figure
	 * @return the title
	 */
	String getTitle();
	
	/**
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the displacement recording continue, false otherwise
	 */
	boolean getRotationDisplacement(int[] displacement);

	/**
	 * Ansynchrnous stop of rotation tracking.
	 */
	void stopRotationRecording();
	
	/**
	 * Put the figure in top of other windows
	 */
	void showWindow();
	
	/**
	 * Disable canvas
	 */
	void disableCanvas();
	
	/**
     * If the window does not already contains a 3D canvas,
     * add one.
     * @param figureIndex of the figure that need a canvas
     * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
     */
    void openGraphicCanvas(int figureIndex, int antialiasingQuality);
	
	/**
	 * Destroy the canvas if one already exists
	 */
	void closeGraphicCanvas();
	
	
	/**
	 * Set the event handler of the figure
	 * @param command the name of the Scilab function to call
	 */
	void setEventHandler(String command);
	
	/**
	 * Set the status of the event handler of the figure
	 * @param status is true to set the event handler active
	 */
	void setEventHandlerEnabled(boolean status);
	
	/**
	 * Turn on or off single buffering
	 * @param useSingleBuffer if true use single buffer if false use double buffering
	 */
	void setSingleBuffered(boolean useSingleBuffer);
	
	/**
	 * Change the quality for antialiasing.
	 * @param figureIndex index of the figure
	 * @param quality 0 if no antialiasing or the number of passes to use.
	 */
	void setAntialiasingQuality(int figureIndex, int quality);
	
	
}
