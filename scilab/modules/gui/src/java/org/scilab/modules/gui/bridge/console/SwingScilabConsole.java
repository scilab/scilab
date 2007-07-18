
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.console;

import java.awt.Dimension;

import javax.swing.JPanel;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyledDocument;

import org.scilab.modules.console.SciConsole;
import org.scilab.modules.console.SciInputCommandView;
import org.scilab.modules.console.SciPromptView;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import com.artenum.console.util.StringConstants;

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
		this.getConfiguration().getOutputView().setCaretPositionToEnd();
		this.getConfiguration().getOutputView().append(dataToDisplay);
		updateScrollPosition();
	}

	/**
	 * Reads input data in the console
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#readLine()
	 */
	public String readLine() {
		String cmd;
		
		// Show the prompt
		this.getConfiguration().getInputCommandView().setEditable(true);
		this.getConfiguration().getPromptView().setVisible(true);
		
		// Modify the size of the input command view (the prompt was not visible when last size modification done
		if (this.getInputCommandViewSizeForced()) {

			JPanel promptView = ((JPanel) this.getConfiguration().getPromptView());
	
			int height = ((JPanel) promptView.getParent()).getPreferredSize().height;
			int width = ((JPanel) promptView.getParent()).getPreferredSize().width;
			int promptViewHeight = ((SciPromptView) promptView).getPromptUI().getHeight();
			
			/* New dimension for the input command view */
			int newHeight = height + promptViewHeight;
			Dimension newDim = null;
			
			if (newHeight > promptViewHeight) {
				/* If the input command view is bigger than the promptUI */
				/* It's height is descreased */
				newDim = new Dimension(width, newHeight);
			} else {
				/* If the input command view is smaller than the promptUI */
				/* It's height adapted to the promptUI height */
				newDim = new Dimension(width, promptViewHeight);
				this.setInputCommandViewSizeForced(false);
			}
			((JPanel) promptView.getParent()).setPreferredSize(newDim);
			((JPanel) promptView.getParent()).invalidate();
			((JPanel) promptView.getParent()).doLayout();
		}

		// Remove last line returned given by Scilab (carriage return)
		try {
			StyledDocument outputStyledDoc = this.getConfiguration().getOutputViewStyledDocument();			
			int lastEOL = outputStyledDoc.getText(0, outputStyledDoc.getLength()).lastIndexOf(StringConstants.NEW_LINE);
			outputStyledDoc.remove(lastEOL - 2, outputStyledDoc.getLength() - lastEOL + 2);
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		updateScrollPosition();
		
		// Gets the focus to have the caret visible
		this.getConfiguration().getInputCommandView().requestFocus();
		
		// Avoids reading of an empty buffer
		((SciInputCommandView) this.getConfiguration().getInputCommandView()).setBufferProtected();
		
		// Reads the buffer
		cmd = ((SciInputCommandView) this.getConfiguration().getInputCommandView()).getCmdBuffer();
		
		// Gives the focus to the console to avoid having a blinking caret in the not-editable input command view
		this.requestFocus();

		// Hide the prompt
		this.getConfiguration().getInputCommandView().setEditable(false);
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

	/**
	 * Clear the Console
	 */
	public void clear() {
	// TODO : Must do something... but what...
	}
}
