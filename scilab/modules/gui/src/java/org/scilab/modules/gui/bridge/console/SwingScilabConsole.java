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

package org.scilab.modules.gui.bridge.console;

import java.awt.Dimension;

import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyledDocument;

import org.scilab.modules.console.OneCharKeyEventListener;
import org.scilab.modules.console.SciConsole;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.util.StringConstants;

/**
 * Swing implementation for Scilab Console in GUIs
 * This implementation uses JyConsole package
 * @author Vincent COUVERT
 */
public class SwingScilabConsole extends SciConsole implements SimpleConsole {

	private static final long serialVersionUID = 1L;
	
	/**
	 * Constructor
	 */
	public SwingScilabConsole() {
		super();
	}
	
	/**
	 * Displays data in the console
	 * @param dataToDisplay the data to be displayed
	 * @see fr.scilab.console.Console#display(java.lang.String)
	 */
	public void display(String dataToDisplay) {
		this.getConfiguration().getOutputView().append(dataToDisplay);
	}

	/**
	 * This method is used to display the prompt
	 */
	public void displayPrompt() {
		
		InputCommandView inputCmdView = this.getConfiguration().getInputCommandView();
		// Show the prompt
		this.getConfiguration().getPromptView().setVisible(true);

		// Show the input command view and its hidden components
		inputCmdView.setEditable(true);

		((JTextPane) inputCmdView).setCaretColor(((JTextPane) inputCmdView).getForeground());
		
		// Remove last line returned given by Scilab (carriage return)
		try {
			StyledDocument outputStyledDoc = this.getConfiguration().getOutputViewStyledDocument();			
			int lastEOL = outputStyledDoc.getText(0, outputStyledDoc.getLength()).lastIndexOf(StringConstants.NEW_LINE);

			if (lastEOL > 1) { // To avoid a "javax.swing.text.BadLocationException: Invalid remove" exception
				outputStyledDoc.remove(lastEOL, outputStyledDoc.getLength() - lastEOL);
			}
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		
		updateScrollPosition();
	}

	/**
	 * Reads one user input char
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#getCharWithoutOutput()
	 */
	public int getCharWithoutOutput() {
		int retChar;
		
		updateScrollPosition();
		
		// Avoids reading of an empty buffer
		try {
			((SciConsole) this).getCanReadUserInputValue().acquire();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// Add a keylistener which will set the returned char
		OneCharKeyEventListener keyListener = new OneCharKeyEventListener(this);
		((JTextPane) this.getConfiguration().getInputCommandView()).addKeyListener(keyListener);
		((JTextPane) this.getConfiguration().getOutputView()).addKeyListener(keyListener);
		
		// Reads the buffer
		retChar = this.getUserInputValue();
		((SciConsole) this).getCanReadUserInputValue().release();

		// Remove the "more" message and replace it by an empty line
		this.clear(-1);
		this.display(StringConstants.NEW_LINE);

		// Remove the key listener
		((JTextPane) this.getConfiguration().getInputCommandView()).removeKeyListener(keyListener);
		((JTextPane) this.getConfiguration().getOutputView()).removeKeyListener(keyListener);
		
		// Send back the focus the the input view
		this.getConfiguration().getInputCommandView().requestFocus();
		((JTextPane) this.getConfiguration().getInputCommandView()).getCaret().setVisible(true);

		return retChar;
	}

	/**
	 * Draw a console
	 */
	public void draw() {
		super.setVisible(true);
		super.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @return the size of the console
	 */
	public Size getDims() {
		return new Size(super.getWidth(), super.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @return the position of the console
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Gets the visibility status of a console
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 */
	public boolean isVisible() {
		return super.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param newSize the size we want to set to the console
	 */
	public void setDims(Size newSize) {
		this.setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param newPosition the position we want to set to the console
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param newVisibleState the visibility status we want to set to the console
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
	}

	/**
	 * Clears the Console
	 */
	public void clear() {
		super.clear();
	}

	/**
	  * Sets the prompt displayed in the console
	  * @param prompt the prompt to be displayed in the console
	  */
	public void setPrompt(String prompt) {
		this.getConfiguration().getPromptView().setDefaultPrompt(prompt);
	}
	
}
