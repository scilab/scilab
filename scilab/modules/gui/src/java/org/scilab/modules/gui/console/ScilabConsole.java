
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab Console in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabConsole extends ScilabDockable implements Console {

	private SimpleConsole component;
	
	/**
	 * Constructor
	 */
	protected ScilabConsole() {
		component = ScilabBridge.createConsole();
	}

	/**
	 * Creates a Scilab Console
	 * @return the created console
	 */
	public static Console createConsole() {
		return new ScilabConsole();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleConsole getAsSimpleConsole() {
		return component;
	}
	
	/**
	 * Sets a MenuBar to an element
	 * @param newMenuBar the MenuBar to set to the element
	 */
	public void addMenuBar(MenuBar newMenuBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets a ToolBar to an element
	 * @param newToolBar the ToolBar to set to the element
	 */
	public void addToolBar(ToolBar newToolBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Displays data in the console
	 * @param dataToDisplay the data to be displayed
	 * @see fr.scilab.console.Console#display()
	 */
	public void display(String dataToDisplay) {
		ScilabBridge.display(component, dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @return the data entered by the user
	 * @see org.scilab.modules.shell.Console#readLine()
	 */
	public String readLine() {
		return ScilabBridge.readLine(component);
	}

	/**
	 * Draw a console
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(component);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @return the size of the console
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(component);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @return the position of the console
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(component);
	}

	/**
	 * Gets the visibility status of a console
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(component);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param newSize the size we want to set to the console
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(component, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param newPosition the position we want to set to the console
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(component, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param newVisibleState the visibility status we want to set to the console
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(component, newVisibleState);
	}

	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(Container container) {
		//return ScilabBridge.addAsMemberTo(component, container);
		throw new UnsupportedOperationException();
	}
}
