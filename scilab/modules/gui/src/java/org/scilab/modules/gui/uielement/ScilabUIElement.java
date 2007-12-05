
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.uielement;

import org.scilab.modules.gui.utils.Layout;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;


/**
 * Abstract class defining an object in Scilab GUIs
 * In the following, an UIElement is an object in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Marouane BEN JELLOUL
 */
public abstract class ScilabUIElement implements UIElement {

	/**
	 * The toolBar associated to the UIElement
	 */
	private ToolBar toolBar;

	/**
	 * The Id of the ToolBar associated to the UIElement
	 */
	private int toolBarId;

	/**
	 * The menubar associated to the UIElement
	 */
	private MenuBar menuBar;

	/**
	 * The Id of the MenuBar associated to the UIElement
	 */
	private int menuBarId;

	/**
	 * The Id associated to the UIElement
	 */
	//private int elementId;

	/**
	 * The layout of the foreground of the UIElement
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	private Layout foregroundLayout;

	/**
	 * The layout of the background of the UIElement
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	private Layout backgroundLayout;

	/**
	 * The layout of the text of the UIElement
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	private Layout textLayout;

	/***********************/
	/* GETTERS AND SETTERS */
	/***********************/

	/**
	 * Gets the background layout of an UIElement
	 * @return the background layout of the UIElement
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	public Layout getBackgroundLayout() {
		return backgroundLayout;
	}

	/**
	 * Sets the background layout of an UIElement
	 * @param backgroundLayout the backgroundLayout to set
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	public void setBackgroundLayout(Layout backgroundLayout) {
		this.backgroundLayout = backgroundLayout;
	}

	/**
	 * Gets the foreground layout of an UIElement
	 * @return the foreground layout of the UIElement
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	public Layout getForegroundLayout() {
		return foregroundLayout;
	}

	/**
	 * Sets the foreground layout of an UIElement
	 * @param foregroundLayout the foregroundLayout to set
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	public void setForegroundLayout(Layout foregroundLayout) {
		this.foregroundLayout = foregroundLayout;
	}

	/**
	 * Sets the menubar associated to an UIElement
	 * @param menuBar the menuBar to set
	 */
	public void addMenuBar(MenuBar menuBar) {
		this.menuBar = menuBar;
		// When the menuBar is added to an element, 
		// we also have to update the menuBarId of this element
		if (menuBar != null) {
			setMenuBarId(menuBar.getAsSimpleMenuBar().getElementId());
		}
	}

	/**
	 * Get the Id of the menubar in the UIElementMapper
	 * @return the Id of the UIElement
	 */
	public int getMenuBarId() {
		return menuBarId;
	}
	
	/**
	 * Set the Id of the menubar in the UIElementMapper
	 * @param id the Id of the UIElement
	 */
	public void setMenuBarId(int id) {
		this.menuBarId = id;
	}

	/**
	 * Get the Id of the toolbar in the UIElementMapper
	 * @return the Id of the UIElement
	 */
	public int getToolBarId() {
		return toolBarId;
	}
	
	/**
	 * Set the Id of the toolbar in the UIElementMapper
	 * @param id the Id of the UIElement
	 */
	public void setToolBarId(int id) {
		this.toolBarId = id;
	}

	/**
	 * Gets the layout of the text of an UIElement
	 * @return the textLayout
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	public Layout getTextLayout() {
		return textLayout;
	}

	/**
	 * Sets the layout of the text of an UIElement
	 * @param textLayout the textLayout to set
	 * @see org.scilab.modules.gui.utils.Layout
	 */
	public void setTextLayout(Layout textLayout) {
		this.textLayout = textLayout;
	}

	/**
	 * Gets the toolbar associated to an UIElement
	 * @return the toolBar
	 */
	public ToolBar getToolBar() {
		return toolBar;
	}

	/**
	 * Sets the toolbar associated to an UIElement
	 * @param toolBar the toolBar to set
	 */
	public void addToolBar(ToolBar toolBar) {
		this.toolBar = toolBar;
		// When the toolBar is added to an element, 
		// we also have to update the toolBarId of this element
		if (toolBar != null) {
			setToolBarId(toolBar.getAsSimpleToolBar().getElementId());
		}
	}

	/*******************************/
	/* UIElement interface methods */
	/*******************************/

	/**
	 * Draws an UIElement
	 */
	public abstract void draw();
}
