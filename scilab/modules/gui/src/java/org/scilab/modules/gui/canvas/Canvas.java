
/* Copyright INRIA 2007 */

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
	 
}
