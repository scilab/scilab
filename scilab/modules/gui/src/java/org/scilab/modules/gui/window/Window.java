
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.uielement.UIElement;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * Interface for Scilab GUIs windows
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public interface Window extends UIElement {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	WindowBridge getWindowBridge();
	
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
//	void addTab(Tab newTab);
	
	/**
	 * Sets a MenuBar to a window
	 * @param newMenuBar the MenuBar to add to the window
	 */
	void setMenuBar(MenuBar newMenuBar);
	
	/**
	 * Get a MenuBar from this window
	 * @return this window MenuBar
	 */
	MenuBar getMenuBar();
	
	/**
	 * Sets a ToolBar to a window
	 * @param newToolBar the ToolBar to set to the window
	 */
	void setToolBar(ToolBar newToolBar);
	
	/**
	 * Get a ToolBar from this window
	 * @return this window ToolBar
	 */
	ToolBar getToolBar();
	
	/**
	 * Sets a InfoBar to a window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 */
	void setInfoBar(TextBox newInfoBar);
}
