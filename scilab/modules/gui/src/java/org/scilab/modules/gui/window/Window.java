
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.uielement.UIElement;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;

/**
 * Interface for Scilab GUIs windows
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public interface Window extends UIElement {
	
	/**
	 * Gets a much more dummy Objects
	 * @return the component.
	 */
	SimpleWindow getAsSimpleWindow();
	
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
	
	/**
	 * Sets a InfoBar to a window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 */
	void addInfoBar(TextBox newInfoBar);
	
	/**
	 * Get a InfoBar from this window
	 * @return this window InfoBar
	 */
	TextBox getInfoBar();
}
