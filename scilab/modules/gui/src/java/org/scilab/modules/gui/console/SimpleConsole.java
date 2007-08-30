
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;


/**
 * Interface for SimpleConsole the associated object to Scilab GUIs Console
 * @author Marouane BEN JELLOUL
 */
public interface SimpleConsole {

	/**
	 * Reads input data in console
	 * @return the string entered by user
	 */
	String readLine();

	/**
	 * Displays data in Console
	 * @param dataToDisplay the data to be displayed
	 */
	void display(String dataToDisplay);
	
	/**
	 * Clears the console (no more scroll history and prompt on top left corner)
	 */
	void clear();
	
	/**
   * Clears lines from the end of the output view
   * @param nbLines the number of lines to be deleted
   */
  void clear(int nbLines);
  
  /**
   * Puts the prompt in the top left corner of the console
   */
  void toHome();
	
	/**
	 * Gets the size of an Console (width and height)
	 * @return the size of the Console
	 */
	Size getDims();

	/**
	 * Sets the size of an Console (width and height)
	 * @param newSize the size we want to set to the Console
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an Console (X-coordinate and Y-corrdinate)
	 * @return the position of the Console
	 */
	Position getPosition();

	/**
	 * Sets the position of an Console (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the Console
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an Console
	 * @return the visibility status of the Console (true if the Console is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an Console
	 * @param newVisibleState the visibility status we want to set for the Console
	 * 			(true if the Console is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);
	
	/**
	 * Reads one user input char
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#getCharWithoutOutput()
	 */
	int getCharWithoutOutput();

	/**
	 * Draws a Console
	 */
	void draw();
}
