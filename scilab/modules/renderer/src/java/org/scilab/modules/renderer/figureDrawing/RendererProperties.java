/*------------------------------------------------------------------------*/
/* file: RendererProperties.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Properties needed by a figure from its rendering canvas         */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.figureDrawing;

import javax.media.opengl.GL;

/**
 * Properties needed by a figure from its rendering canvas
 * @author Jean-Baptiste Silvy
 */
public interface RendererProperties {

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
	 */
	void setCanvasSize(int width, int height);
	
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
	
}
