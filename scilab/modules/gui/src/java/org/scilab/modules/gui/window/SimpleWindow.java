
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.uielement.SimpleUIElement;

/**
 * Interface for SimpleWindow the associated object to Scilab GUIs Window
 * @author Marouane BEN JELLOUL
 */
public interface SimpleWindow extends SimpleUIElement {
	
	/**
	 * Gets the title of a window
	 * @return the title of the window
	 */
	String getTitle();

	/**
	 * Sets the title of a window
	 * @param newWindowTitle the title we want to set for the window
	 */
	void setTitle(String newWindowTitle);

	/**
	 * Add a tab to a window
	 * @param newTab the tab to add to the window
	 */
//	 TODO uncomment when SimpleTab ready
//	void addTab(SimpleTab newTab);
	
	/**
	 * Sets a InfoBar to a window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 */
	void addInfoBar(SimpleTextBox newInfoBar);
}
