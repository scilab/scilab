
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.UIElement;
import org.scilab.modules.gui.tab.Tab;

/**
 * Interface for Scilab GUIs windows
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public interface Window extends UIElement {

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
	void addTab(Tab newTab);
}
