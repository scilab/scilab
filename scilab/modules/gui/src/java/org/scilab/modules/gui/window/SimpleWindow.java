
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.menubar.MenuBarBridge;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.uielement.UIElementBridge;

/**
 * Interface for WindowBridge the associated object to Scilab GUIs Window
 * @author Marouane BEN JELLOUL
 */
public interface SimpleWindow extends UIElementBridge {
	
	/**
	 * Gets the title of a windowBridge
	 * @return the title of the windowBridge
	 */
	String getTitle();

	/**
	 * Sets the title of a windowBridge
	 * @param newWindowTitle the title we want to set for the windowBridge
	 */
	void setTitle(String newWindowTitle);

	/**
	 * Add a tab to a windowBridge
	 * @param newTab the tab to add to the windowBridge
	 */
//	 TODO uncomment when TabBridge ready
//	void addTab(TabBridge newTab);
	
	/**
	 * Sets a MenuBarBridge to a windowBridge
	 * @param newMenuBar the MenuBarBridge to add to the windowBridge
	 */
	void setMenuBar(MenuBarBridge newMenuBar);

//	TODO remove because getMenuBar handled by ScilabWindow
//	/**
//	 * Get a MenuBarBridge from this windowBridge
//	 * @return this windowBridge MenuBarBridge
//	 */
//	MenuBarBridge getMenuBar();
	
	/**
	 * Sets a ToolBarBridge to a windowBridge
	 * @param newToolBar the ToolBarBridge to set to the windowBridge
	 */
	void setToolBar(SimpleToolBar newToolBar);
	
//	TODO remove because getToolBar handled by ScilabWindow
//	/**
//	 * Get a ToolBarBridge from this windowBridge
//	 * @return this windowBridge ToolBarBridge
//	 */
//	ToolBarBridge getToolBar();
	
	/**
	 * Sets a InfoBar to a window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 */
//	 TODO uncomment when TextBoxBridge ready
//	void setInfoBar(TextBoxBridge newInfoBar);
}
