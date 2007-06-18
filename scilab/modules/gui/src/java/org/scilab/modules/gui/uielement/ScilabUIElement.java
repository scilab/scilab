
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.uielement;

import org.scilab.modules.gui.utils.Layout;
import org.scilab.modules.gui.widget.MenuBar;
import org.scilab.modules.gui.widget.ToolBar;


/**
 * Abstract class defining an object in Scilab GUIs
 * In the following, an UIElement is an object in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public abstract class ScilabUIElement implements UIElement {

	/**
	 * The toolBar associated to the UIElement
	 */
	private ToolBar toolBar;

	/**
	 * The menubar associated to the UIElement
	 */
	private MenuBar menuBar;

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
	 * Gets the menubar associated to an UIElement
	 * @return the menuBar associated to the UIElement
	 */
	public MenuBar getMenuBar() {
		return menuBar;
	}

	/**
	 * Sets the menubar associated to an UIElement
	 * @param menuBar the menuBar to set
	 */
	public void setMenuBar(MenuBar menuBar) {
		this.menuBar = menuBar;
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
	public void setToolBar(ToolBar toolBar) {
		this.toolBar = toolBar;
	}

	/*******************************/
	/* UIElement interface methods */
	/*******************************/

	/**
	 * Draws an UIElement
	 */
	public abstract void draw();

}
