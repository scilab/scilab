
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.canvas;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab Canvas in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabCanvas extends ScilabDockable implements Canvas {

	private SimpleCanvas component;
	
	/**
	 * Constructor
	 */
	protected ScilabCanvas() {
		component = ScilabBridge.createCanvas();
	}

	/**
	 * Creates a Scilab Canvas
	 * @return the created canvas
	 */
	public static Canvas createCanvas() {
		return new ScilabCanvas();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleCanvas getAsSimpleCanvas() {
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
	 * Draws a Scilab canvas
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(component);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 * @return the size of the canvas
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(component);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @return the position of the canvas
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(component);
	}

	/**
	 * Gets the visibility status of a Scilab Canvas
	 * @return the visibility status of the canvas (true if the canvas is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(component);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 * @param newSize the size we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(component, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @param newPosition the position we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(component, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Canvas
	 * @param newVisibleState the visibility status we want to set to the canvas (true to set the canvas visible, false else)
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
