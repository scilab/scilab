
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.text.SimpleText;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for TextBoxBridge the associated object to Scilab GUIs TextBox
 * @author Marouane BEN JELLOUL
 */
public interface SimpleTextBox extends SimpleText {

	/**
	 * Gets the size of an TextBox (width and height)
	 * @return the size of the TextBox
	 */
	Size getDims();

	/**
	 * Sets the size of an TextBox (width and height)
	 * @param newSize the size we want to set to the TextBox
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an TextBox (X-coordinate and Y-corrdinate)
	 * @return the position of the TextBox
	 */
	Position getPosition();

	/**
	 * Sets the position of an TextBox (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the TextBox
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an TextBox
	 * @return the visibility status of the TextBox (true if the TextBox is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an TextBox
	 * @param newVisibleState the visibility status we want to set for the TextBox
	 * 			(true if the TextBox is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an TextBox
	 */
	void draw();
	
}
