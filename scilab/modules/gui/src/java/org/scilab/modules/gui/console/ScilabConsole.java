
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab Console in GUIs
 * @author Vincent COUVERT
 */
public class ScilabConsole implements Console {

	/**
	 * Constructor
	 */
	protected ScilabConsole() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Console
	 * @return the created console
	 */
	public static Console createConsole() {
		return ScilabBridge.createConsole();
	}

	/**
	 * Displays data in the console
	 * @param dataToDisplay the data to be displayed
	 * @see fr.scilab.console.Console#display()
	 */
	public void display(String dataToDisplay) {
		ScilabBridge.display(this, dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @return the data entered by the user
	 * @see org.scilab.modules.shell.Console#readLine()
	 */
	public String readLine() {
		return ScilabBridge.readLine(this);
	}

	/**
	 * Draw a console
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(this);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @return the size of the console
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(this);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @return the position of the console
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(this);
	}

	/**
	 * Gets the visibility status of a console
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(this);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param newSize the size we want to set to the console
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(this, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param newPosition the position we want to set to the console
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(this, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param newVisibleState the visibility status we want to set to the console
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setPosition(this, newVisibleState);
	}

}
