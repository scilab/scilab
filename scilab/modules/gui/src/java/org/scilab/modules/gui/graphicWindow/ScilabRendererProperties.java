/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
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

package org.scilab.modules.gui.graphicWindow;

import javax.media.opengl.GL;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Debug;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.figureDrawing.RendererProperties;


/**
 * Main class for renderer in Scilab
 * @author Jean-Baptiste Silvy
 */
public class ScilabRendererProperties implements RendererProperties {

    /** Rendering canvas */
    private Canvas parentCanvas;
    /** Enclosing tab */
    private Tab parentTab;


    /**
     * Default constructor
     * @param parentTab the parent tab of this renderer 
     * @param parentCanvas the parent canvas of this renderer or null if none already exists
     * @param figureIndex index of the modified figure
     */
    public ScilabRendererProperties(Tab parentTab, Canvas parentCanvas, int figureIndex) {
	this.parentCanvas = parentCanvas;
	this.parentTab = parentTab;
    }

    private static int nbForce = 0;
    /**
     * Force the parent Canvas to be displayed
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#forceDisplay()
     */
    public void forceDisplay() {
	nbForce++;
	Debug.DEBUG(this.getClass().getSimpleName(), "forceDisplay : "+nbForce);
	parentTab.draw();
    }

    /**
     * Get the parent canvas height
     * @return the height of the parent canvas
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getCanvasHeight()
     */
    public int getCanvasHeight() {
	return parentTab.getAxesSize().getHeight();
    }

    /**
     * Get the parent canvas width
     * @return the width of the parent canvas
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getCanvasWidth()
     */
    public int getCanvasWidth() {
	return parentTab.getAxesSize().getWidth();
    }

    /**
     * Get the GL pipeline of the parent canvas
     * @return the GL of the parent canvas
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getGLPipeline()
     */
    public GL getGLPipeline() {
	if (parentCanvas != null) {
	    return parentCanvas.getGL();
	} else {
	    return null;
	}
    }

    /**
     * Get the name of the parent tab
     * @return the name of the parent tab
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getInfoMessage()
     */
    public String getInfoMessage() {
	return parentTab.getInfoBar().getText();
    }

    /**
     * Get the height of the parent window
     * @return the height of the parent window
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getWindowHeight()
     */
    public int getWindowHeight() {
	return parentTab.getParentWindow().getDims().getHeight();
    }

    /**
     * Get the x-coordinate of the parent window
     * @return the x-coordinate of the parent window
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getWindowPosX()
     */
    public int getWindowPosX() {
	return parentTab.getParentWindow().getPosition().getX();
    }

    /**
     * Get the y-coordinate of the parent window
     * @return the y-coordinate of the parent window
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getWindowPosY()
     */
    public int getWindowPosY() {
	return parentTab.getParentWindow().getPosition().getY();
    }

    /**
     * Get the width of the parent window
     * @return the width of the parent window
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getWindowWidth()
     */
    public int getWindowWidth() {
	return parentTab.getParentWindow().getDims().getWidth();
    }

    /**
     * Set the size of the parent canvas
     * @param width the width to set to the parent
     * @param height the height to set to the parent
     * @return indicates if the size could be successfully modified
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#setCanvasSize(int, int)
     */
    public int setCanvasSize(int width, int height) {

	if (!getAutoResizeMode()) {
	    // autore size off, just resize the canvas
	    try {
		//parentCanvas.setDims(new Size(width, height));
		parentTab.setAxesSize(new Size(width, height));
	    } catch (IllegalArgumentException e) {
		// canvas too large
		return RESIZE_SIZE_TOO_LARGE;
	    }
	} else if (parentTab.getParentWindow().getNbDockedObjects() == 1) {
	    // canvas tab is the only one in its window
	    // so resize window (tab and canvas will follow)

	    Window parentWindow = parentTab.getParentWindow();

	    // to be sure that window and children dimensions are up to date.
	    parentWindow.updateDimensions();


	    Size currentSize = parentTab.getAxesSize();
	    // compute the requested size modifications
	    int deltaX = width - currentSize.getWidth();
	    int deltaY = height - currentSize.getHeight();


	    // apply them to the parent window
	    Size windowSize = parentWindow.getDims();
	    windowSize.setWidth(windowSize.getWidth() + deltaX);
	    windowSize.setHeight(windowSize.getHeight() + deltaY);
	    parentWindow.setDims(windowSize);

	    // also apply on canvas otherwise the canvas is resized twice by Swing
	    try {
		//parentCanvas.setDims(new Size(width, height));
		parentTab.setAxesSize(new Size(width, height));
	    } catch (IllegalArgumentException e) {
		// canvas too large
		return RESIZE_SIZE_TOO_LARGE;
	    }

	} else {
	    // if there are several docked objects, don't allow to resize canvas
	    // if it must changed either the tab or window size
	    return RESIZE_MULTIPLE_DOCKED_TAB;
	}

	return RESIZE_SUCCESS;
    }

    /**
     * Set the name of the parent tab
     * @param infoMessage the name of the parent tab
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getInfoMessage()
     */
    public void setInfoMessage(String infoMessage) {
	parentTab.getInfoBar().setText(infoMessage);
    }

    /**
     * Set the position of the parent window
     * @param posX the x-coordinate of the parent window
     * @param posY the yx-coordinate of the parent window
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#setWindowPosition(int, int)
     */
    public void setWindowPosition(int posX, int posY) {
	parentTab.getParentWindow().setPosition(new Position(posX, posY));
    }

    /**
     * Set the size of the parent window
     * @param width the width to set to the parent
     * @param height the height to set to the parent
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#setWindowSize(int, int)
     */
    public void setWindowSize(int width, int height) {
	parentTab.getParentWindow().setDims(new Size(width, height));

    }

    /**
     * Set the pixmap mode
     * @param onOrOff true ("on" for Scilab) to set auto swap buffer off
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getPixmapMode()
     */
    public void setPixmapMode(boolean onOrOff) {
	// pixmap "on" means auto swap buffer off.
	if (parentCanvas != null) {
	    parentCanvas.setAutoSwapBufferMode(!onOrOff);
	}
    }

    /**
     * Get the pixmap mode
     * @return true ("on" for Scilab) if auto swap buffer is off
     * @see org.scilab.modules.renderer.figureDrawing.RendererProperties#getPixmapMode()
     */
    public boolean getPixmapMode() {
	if (parentCanvas != null) {
	    return !(parentCanvas.getAutoSwapBufferMode());
	} else {
	    return false;
	}
    }

    /**
     * Get the parent tab
     * @return the parent tab
     */
    public Tab getParentTab() {
	return parentTab;
    }

    /**
     * Get the parent canvas
     * @return the parent canvas
     */
    public Canvas getCanvas() {
	return parentCanvas;
    }

    /**
     * Close the figure
     */
    public void closeFigure() {
	// hide tab before to avoid unwanted display
	parentTab.setVisible(false);

	parentTab.close();
    }

    /**
     * Specify wether the canvas should fit the parent tab size
     * (and consequently the scrollpane size) or not
     * @param onOrOff true to enable autoresize mode
     */
    public void setAutoResizeMode(boolean onOrOff) {
	parentTab.setAutoResizeMode(onOrOff);
    }

    /**
     * @return wether the resize mode is on or off
     */
    public boolean getAutoResizeMode() {
	return parentTab.getAutoResizeMode();
    }

    /**
     * Get the part of the canvas which is currently viewed
     * @return [x,y,w,h] array
     */
    public int[] getViewport() {
	return parentTab.getViewingRegion();
    }

    /**
     * Specify a new viewport for the canvas
     * For SwingScilabCanvas viewport can not be modified
     * since it match the parent tab size
     * @param posX X coordinate of upper left point of the viewport within the canvas
     * @param posY Y coordinate of upper left point of the viewport within the canvas
     * @param width width of the viewport
     * @param height height of the viewport
     */
    public void setViewport(int posX, int posY, int width, int height) {
	parentTab.setViewingRegion(posX, posY, width, height);
    }

    /**
     * Set the background of the Canvas.
     * @param red red channel
     * @param green green channel 
     * @param blue blue channel
     */
    public void setBackgroundColor(double red, double green, double blue) {
	if (parentCanvas != null) {
	    parentCanvas.setBackgroundColor(red, green, blue);
	}
	parentTab.setBackground(red, green, blue);
    }

    /**
     * Create an interactive selection rectangle and return its pixel coordinates
     * @param figureIndex index of the figure on which the rubber box is applied
     * @param isClick specify whether the rubber box is selected by one click for each one of the two edge
     *                or a sequence of press-release
     * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
     * @param initialRect if not null specify the initial rectangle to draw
     * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
     * @return Scilab code of the pressed button
     */
    public int rubberBox(int figureIndex, boolean isClick, boolean isZoom, int[] initialRect, int[] endRect) {
		// rubber box needs an OpenGL canvas to display the selection rectangle.
		if (parentCanvas == null) {
		    openGraphicCanvas(figureIndex, 0);
		}
		return parentCanvas.rubberBox(isClick, isZoom, initialRect, endRect);
    }

    /**
     * Set the title of the figure
     * @param title the title
     */
    public void setTitle(String title) {
	parentTab.setName(title);
    }

    /**
     * Get the title of the figure
     * @return the title
     */
    public String getTitle() {
	return parentTab.getName();
    }

    /**
     * Get the displacement in pixel that should be used for rotating axes
     * @param displacement out parameter, [x,y] array of displacement in pixels
     * @return true if the diplacement recording continue, false otherwise
     */
    public boolean getRotationDisplacement(int[] displacement) {
	// if there is't any canvas opened, we use the parentTab instead
	// for tracking the displacement
	return parentTab.getRotationDisplacement(displacement);

    }

    /**
     * Asynchronous stop of rotation tracking.
     */
    public void stopRotationRecording() {
	parentTab.stopRotationRecording();
    }

    /**
     * Put the figure in top of other windows
     */
    public void showWindow() {
	parentTab.getParentWindow().raise();
    }

    /**
     * Disable canvas
     */
    public void disableCanvas() {
	if (parentCanvas != null) {
	    parentCanvas.close();
	}
    }

    /**
     * If the window does not already contains a 3D canvas,
     * add one.
     * @param figureIndex of the figure that need a canvas
     * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
     */
    public void openGraphicCanvas(int figureIndex, int antialiasingQuality) {
    	if (parentCanvas == null) {

    		// create canvas
    		parentCanvas = ScilabCanvas.createCanvas(figureIndex, antialiasingQuality);

    		// add it to the tab
    		parentTab.addMember(parentCanvas);

    	}
    }

    /**
     * Destroy the canvas if one already exists
     */
    public void closeGraphicCanvas() {
    	if (parentCanvas != null) {
    		// disable the canvas
    		parentCanvas.close();

    		// remove it from the tab
    		parentTab.removeMember(parentCanvas);
    	}
    	parentCanvas = null;
    }

    /**
     * Set the event handler of the figure
     * @param command the name of the Scilab function to call
     */
    public void setEventHandler(String command) {
	parentTab.setEventHandler(command);
    }

    /**
     * Set the status of the event handler of the figure
     * @param status is true to set the event handler active
     */
    public void setEventHandlerEnabled(boolean status) {
	parentTab.setEventHandlerEnabled(status);
    }
    
    /**
	 * Turn on or off single buffering
	 * @param useSingleBuffer if true use single buffer if false use double buffering
	 */
	public void setSingleBuffered(boolean useSingleBuffer) {
		if (parentCanvas != null) {
			parentCanvas.setSingleBuffered(useSingleBuffer);
		}
	}
	
	/**
	 * Change the quality for antialiasing.
	 * @param figureIndex index of the figure
	 * @param quality 0 if no antialiasing or the number of passes to use.
	 */
	public void setAntialiasingQuality(int figureIndex, int quality) {
		// we need to close the canvas and open a new to change
		// the quality
		if (parentCanvas != null) {
			closeGraphicCanvas();
			openGraphicCanvas(figureIndex, quality);
		}
	}

}
