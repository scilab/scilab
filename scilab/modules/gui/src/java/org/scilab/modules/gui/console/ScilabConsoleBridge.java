/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.console;

import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.tab.Tab;
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
	 * @see fr.scilab.console.HelpBrowser#display()
	 */
	public static void display(Console console, String dataToDisplay) {
		console.getAsSimpleConsole().display(dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @param console the console to read the data from
	 * @return the data entered by the user
	 * @see fr.scilab.console.HelpBrowser#readLine()
	 */
	public static String readLine(Console console) {
		return console.getAsSimpleConsole().readLine();
	}

	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public static int addMember(Tab tab, Frame member) {
		return tab.addMember(member);
	}

	/**
	 * Draw a console
	 * @param console the console to draw
	 */
	public static void draw(Console console) {
		console.getAsSimpleConsole().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @param console the console which we want to get the dimensions of
	 * @return the size of the console
	 */
	public static Size getDims(Console console) {
		return console.getAsSimpleConsole().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @param console the console which we want to get the position of
	 * @return the position of the console
	 */
	public static Position getPosition(Console console) {
		return console.getAsSimpleConsole().getPosition();
	}

	/**
	 * Gets the visibility status of a console
	 * @param console the console which we want to get the visibility status of
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 */
	public static boolean isVisible(Console console) {
		return console.getAsSimpleConsole().isVisible();
	}
	
	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param console the console which we want to set the dimensions of
	 * @param newSize the size we want to set to the console
	 */
	public static void setDims(Console console, Size newSize) {
		console.getAsSimpleConsole().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param console the console which we want to set the position of
	 * @param newPosition the position we want to set to the console
	 */
	public static void setPosition(Console console, Position newPosition) {
		console.getAsSimpleConsole().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param console the console which we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the console
	 */
	public static void setVisible(Console console, boolean newVisibleState) {
		console.getAsSimpleConsole().setVisible(newVisibleState);
	}
	
	/**
	 * Clears the console (no more scroll history and prompt on top left corner)
	 * @param console the console to be cleared
	 */
	public static void clear(Console console) {
		console.getAsSimpleConsole().clear();
	}
	
	/**
   * Clears lines from the end of the output view
   * @param console the console to be cleared
   * @param nbLines the number of lines to be deleted
   */
  public static void clear(Console console, int nbLines) {
		console.getAsSimpleConsole().clear(nbLines);
	}
	
	/**
   * Puts the prompt in the top left corner of the console
   * @param console the console used
   */
  public static void toHome(Console console) {
  	console.getAsSimpleConsole().toHome();
  }


	/**
	 * Reads one user input char
	 * @param console the console used to get the char
	 * @return the data entered by the user
	 * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
	 */
	public static int getCharWithoutOutput(Console console) {
		return console.getAsSimpleConsole().getCharWithoutOutput();
	}

	/**
	   * Sets the prompt displayed in the console
	   * @param console the console where the prompt is set
	   * @param prompt the prompt to be displayed in the console
	   */
	public static void setPrompt(Console console, String prompt) {
		console.getAsSimpleConsole().setPrompt(prompt);
	}
	
	/**
	 * Updates Scilab internal variables containing the size of the console
	 * These variables are used to format data before displaying it
 	 * @param console object we want to get the size of
 	 */
	public static void scilabLinesUpdate(Console console) {
		console.getAsSimpleConsole().scilabLinesUpdate();
	}
	
	/**
	 * Get the current status of the console
	 * If the prompt view is visible, Scilab is waiting for commands
 	 * @param console object we want to get the status of
	 * @return true is Scilab is waiting for commands
	 */
	public static boolean isWaitingForInput(Console console) {
		return console.getAsSimpleConsole().isWaitingForInput();
	}

	/**
	 * Clear the commands history
	 * @param console the console we want to clear the history of 
	 */
	public static void clearHistory(Console console) {
		console.getAsSimpleConsole().clearHistory();
	}

}
