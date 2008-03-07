/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.canvas;

import javax.media.opengl.GL;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab canvas in GUIs
 * @author Vincent COUVERT
 */
public interface Canvas extends Dockable {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleCanvas getAsSimpleCanvas();
	
	/**
	 * Force the canvas to render itself immediately.
	 */
	 void display();
	 
	 /**
	  * Get the GL pipeline used by the canvas
	  * @return GL pipeline
	  */
	 GL getGL();
	 
	 
	 /**
	  * @param onOrOff set wether the swap buffer mode is on or off.
	  */
	 void setAutoSwapBufferMode(boolean onOrOff);
	
	 /**
	  * @return set wether the swap buffer mode is on or off.
	  */
	 boolean getAutoSwapBufferMode();
	 
	 /**
	  * Force the canvas to render itself with synchronisation with its OpenGL process.
	  */
	 void repaint();
	 
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
	 int[] getViewingRegion();
	 
	 /**
	  * Specify a new viewport for the canvas
	  * For SwingScilabCanvas viewport can not be modified
	  * since it match the parent tab size
	  * @param posX X coordinate of upper left point of the viewport within the canvas
	  * @param posY Y coordinate of upper left point of the viewport within the canvas
	  * @param width width of the viewport
	  * @param height height of the viewport
	  */
	 void setViewingRegion(int posX, int posY, int width, int height);
	 
	 /**
	  * Set the background of the Canvas.
	  * @param red red channel
	  * @param green green channel 
	  * @param blue blue channel
	  */
	 void setBackgroundColor(double red, double green, double blue);
	 
	 /**
	  * Set the event handler of the Canvas
	  * @param command the name of the Scilab function to call
	  */
	 void setEventHandler(String command);

	 /**
	  * Set the status of the event handler of the Canvas
	  * @param status is true to set the event handler active
	  */
	 void setEventHandlerEnabled(boolean status);
}
