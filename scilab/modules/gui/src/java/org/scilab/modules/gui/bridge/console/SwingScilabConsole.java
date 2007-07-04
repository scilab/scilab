
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.console;

import java.awt.Dimension;

import org.scilab.modules.console.SciConsole;
import org.scilab.modules.console.SciInputCommandView;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Console in GUIs
 * This implementation uses JyConsole package
 * @author Vincent COUVERT
 */
public class SwingScilabConsole extends SciConsole implements Console {

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
		this.getConfiguration().getOutputView().setCaretPositionToEnd();
		this.getConfiguration().getOutputView().append(dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#readLine()
	 */
	public String readLine() {
		String cmd;
		
		// Show the prompt
		this.getConfiguration().getInputCommandView().setVisible(true);
		this.getConfiguration().getPromptView().setVisible(true);
		
		// To be sure the prompt is not below the bottom of the console
		this.getConfiguration().getInputCommandView().setText(" ");
		this.getConfiguration().getInputCommandView().backspace();
		
		// Gets the focus to have th caret visible
		this.getConfiguration().getInputCommandView().requestFocus();
		
		// Avoids reading of an empty buffer
		((SciInputCommandView) this.getConfiguration().getInputCommandView()).setBufferProtected();
		
		// Reads the buffer
		cmd = ((SciInputCommandView) this.getConfiguration().getInputCommandView()).getCmdBuffer();

		// Hide the prompt
		this.getConfiguration().getInputCommandView().setVisible(false);
		this.getConfiguration().getPromptView().setVisible(false);
	
		return cmd;
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
}
