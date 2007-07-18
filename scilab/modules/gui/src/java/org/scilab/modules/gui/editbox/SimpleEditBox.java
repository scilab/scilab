
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.editbox;

import org.scilab.modules.gui.text.SimpleText;
import org.scilab.modules.gui.utils.Size;


/**
 * Interface for EditBoxBridge the associated object to Scilab GUIs EditBox
 * @author Marouane BEN JELLOUL
 */
public interface SimpleEditBox extends SimpleText {

	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	void setSize(Size dimension);
	
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	Size getSize();
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	void setPosition(int x, int y);
	
}
