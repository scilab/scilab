
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.dockable.ScilabDockable;

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
		ScilabBridge.setVisible(this, newVisibleState);
	}

	/**
	 * Clears the console (no more scroll history and prompt on top left corner)
	 */
	public void clear() {
		ScilabBridge.clear(this);
	}
	
	/**
   * Clears lines from the end of the output view
   * @param nbLines the number of lines to be deleted
   */
  public void clear(int nbLines) {
		ScilabBridge.clear(this, nbLines);
	}
	
	/**
   * Puts the prompt in the top left corner of the console
   */
  public void toHome() {
  	ScilabBridge.toHome(this);
  }

	/**
	 * Reads one user input char
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#getCharWithoutOutput()
	 */
	public int getCharWithoutOutput() {
		return ScilabBridge.getCharWithoutOutput(this);
	}

    /**
	   * Sets the prompt displayed in the console
	   * @param prompt the prompt to be displayed in the console
	   */
	public void setPrompt(String prompt) {
		ScilabBridge.setPrompt(this, prompt);
	}
}
