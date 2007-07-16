
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.text;

import org.scilab.modules.gui.widget.SimpleWidget;

/**
 * Interface for TextBridge the associated object to Scilab GUIs Text
 * @author Marouane BEN JELLOUL
 */
public interface TextBridge extends SimpleWidget {

	/**
	 * Sets the text of a Text Widget
	 * @param newText the text to set to the Text Widget
	 */
	void setText(String newText);

	/**
	 * Gets the text of a Text Widget
	 * @return the text of the Text Widget
	 */
	String getText();
}
