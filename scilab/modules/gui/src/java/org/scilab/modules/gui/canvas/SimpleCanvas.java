
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.canvas;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;


/**
 * Interface for SimpleCanvas the associated object to Scilab GUIs Canvas
 * @author Marouane BEN JELLOUL
 */
public interface SimpleCanvas {

	/**
	 * Gets the size of an Canvas (width and height)
	 * @return the size of the Canvas
	 */
	Size getDims();

	/**
	 * Sets the size of an Canvas (width and height)
	 * @param newSize the size we want to set to the Canvas
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an Canvas (X-coordinate and Y-corrdinate)
	 * @return the position of the Canvas
	 */
	Position getPosition();

	/**
	 * Sets the position of an Canvas (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the Canvas
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an Canvas
	 * @return the visibility status of the Canvas (true if the Canvas is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an Canvas
	 * @param newVisibleState the visibility status we want to set for the Canvas
	 * 			(true if the Canvas is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an Canvas
	 */
	void draw();
	
}
