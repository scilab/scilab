/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
