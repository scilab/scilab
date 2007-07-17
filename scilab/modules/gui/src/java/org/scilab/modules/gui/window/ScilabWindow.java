
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.menubar.MenuBar;

/**
 * Class for Scilab Windows in GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class ScilabWindow extends ScilabUIElement implements Window {
	
	private SimpleWindow component;
		
	private TextBox infoBar;
	
	/**
	 * Constructor
	 */
	protected ScilabWindow() {
		component = ScilabBridge.createWindow();
		super.addMenuBar(null);
		super.addToolBar(null);
		this.infoBar = null;
	}

	/**
	 * Creates a Scilab window object
	 * @return the created window
	 */
	public static Window createWindow() {
		return new ScilabWindow();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleWindow getAsSimpleWindow() {
		return component;
	}
	
	/**
	 * Draw a Scilab window
	 * @see org.scilab.modules.gui.ScilabUIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(component);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab window
	 * @return the dimensions of the window
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(component);
	}

	/**
	 * Sets the dimensions of a Scilab window
	 * @param newWindowSize the size we want to set to the window
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newWindowSize) {
		ScilabBridge.setDims(component, newWindowSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab window
	 * @return the position of the window
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(component);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab window
	 * @param newWindowPosition the position we want to set to the window
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newWindowPosition) {
		ScilabBridge.setPosition(component, newWindowPosition);
	}

	/**
	 * Gets the title of a Scilab window
	 * @return the title of the window
	 * @see org.scilab.modules.gui.window.Window#getTitle()
	 */
	public String getTitle() {
		return ScilabBridge.getTitle(component);
	}

	/**
	 * Sets the title of a Scilab window
	 * @param newWindowTitle the title to set to the window
	 * @see org.scilab.modules.gui.window.Window#setTitle(java.lang.String)
	 */
	public void setTitle(String newWindowTitle) {
		ScilabBridge.setTitle(component, newWindowTitle);
	}

	/**
	 * Gets the visibility status of a Scilab window
	 * @return the visibility status of the window (true if the window is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(component);
	}

	/**
	 * Sets the visibility status of a Scilab window
	 * @param newVisibleState the visibility status we want to set to the window (true to set the window visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(component, newVisibleState);
	}

	/**
	 * Add a tab to a Scilab window
	 * @param newTab the tab to add to the window
	 * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
	 */
//	TODO uncomment when TabBridge ready	
//	public void addTab(Tab newTab) {
//		ScilabBridge.addTab(component, newTab.component);
//	}

	/**
	 * Sets a MeunBar to a Scilab window
	 * @param newMenuBar the tab to add to the window
	 * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.widget.MenuBar)
	 */
	public void addMenuBar(MenuBar newMenuBar) {
		super.addMenuBar(newMenuBar);
		// FIXME : Do it a standard way : getAsMenuBar
		ScilabBridge.addMenuBar(component, newMenuBar.getAsSimpleMenuBar());
	}
	
	/**
	 * Sets a Scilab ToolBar to a Scilab window
	 * @param newToolBar the Scilab ToolBar to set to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
	 */
	public void addToolBar(ToolBar newToolBar) {
		super.addToolBar(newToolBar);
		ScilabBridge.addToolBar(component, newToolBar.getAsSimpleToolBar());
	}
	
	/**
	 * Sets a Scilab InfoBar to a Scilab window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#setInfoBar(org.scilab.modules.gui.textbox.TextBox)
	 */
	public void addInfoBar(TextBox newInfoBar) {
		this.infoBar = newInfoBar;
		ScilabBridge.addInfoBar(component, newInfoBar.getAsSimpleTextBox());
	}
	
	/**
	 * Get a Scilab InfoBar from this Scilab window
	 * @return this window InfoBar
	 */
	public TextBox getInfoBar() {
		return this.infoBar;
	}
}
