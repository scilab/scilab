/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
	 * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
	 */
	int getCharWithoutOutput();

	/**
	 * Draws a Console
	 */
	void draw();

	/**
	   * Sets the prompt displayed in the console
	   * @param prompt the prompt to be displayed in the console
	   */
	void setPrompt(String prompt);
	
	/**
	 * Updates Scilab internal variables containing the size of the console
	 * These variables are used to format data before displaying it
 	 */
	void scilabLinesUpdate();

	/**
	 * Get the current status of the console
	 * If the prompt view is visible, Scilab is waiting for commands
	 * @return true is Scilab is waiting for commands
	 */
	boolean isWaitingForInput();

	/**
	 * Send commands to be executed by Scilab (after a copy/paste or drag&drop...)
	 * @param textToExec all text lines to executed
	 * @param displayCmdInOutput flag indicating if the input command has to be displayed in the output view
	 * @param storeInHistory flag indicating if the input command has to be stored in the history
	 */
	void sendCommandsToScilab(String textToExec, boolean displayCmdInOutput, boolean storeInHistory);
	
	/**
	 * Clear the commands history 
	 */
	void clearHistory();
	
	/**
	 * Select all the console contents
	 */
	void selectAll();
	
	/**
	 * Put the console selected text in the clipboard
	 */
	void copyToClipboard();
	
	/**
	 * Paste clipboard contents in Console input line
	 */
	void pasteClipboard();
	
	/**
	 * Cut selected text in the Console input line
	 */
	void cutSelection();
}
