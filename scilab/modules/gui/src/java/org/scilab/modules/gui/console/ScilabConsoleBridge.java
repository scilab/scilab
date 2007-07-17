
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab Console in GUIs
 * @author Vincent COUVERT
 */
public class ScilabConsoleBridge {

	/**
	 * Constructor
	 */
	protected ScilabConsoleBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Creates a Scilab Console
	 * @return the created console
	 */
	public static SimpleConsole createConsole() {
		return new SwingScilabConsole();
	}

	/**
	 * Displays data in the console
	 * @param console the console to display the data in
	 * @param dataToDisplay the data to be displayed
	 * @see fr.scilab.console.Console#display()
	 */
	public static void display(SimpleConsole console, String dataToDisplay) {
		console.display(dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @param console the console to read the data from
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#readLine()
	 */
	public static String readLine(SimpleConsole console) {
		return console.readLine();
	}

	/**
	 * Draw a console
	 * @param console the console to draw
	 */
	public static void draw(SimpleConsole console) {
		console.draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @param console the console which we want to get the dimensions of
	 * @return the size of the console
	 */
	public static Size getDims(SimpleConsole console) {
		return console.getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @param console the console which we want to get the position of
	 * @return the position of the console
	 */
	public static Position getPosition(SimpleConsole console) {
		return console.getPosition();
	}

	/**
	 * Gets the visibility status of a console
	 * @param console the console which we want to get the visibility status of
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 */
	public static boolean isVisible(SimpleConsole console) {
		return console.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param console the console which we want to set the dimensions of
	 * @param newSize the size we want to set to the console
	 */
	public static void setDims(SimpleConsole console, Size newSize) {
		console.setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param console the console which we want to set the position of
	 * @param newPosition the position we want to set to the console
	 */
	public static void setPosition(SimpleConsole console, Position newPosition) {
		console.setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param console the console which we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the console
	 */
	public static void setVisible(SimpleConsole console, boolean newVisibleState) {
		console.setVisible(newVisibleState);
	}
	
	/**
	 * Clears the console (no more scroll history and prompt on top left corner)
	 */
	public static void clear(Console console) {
		console.clear();
	}

}
