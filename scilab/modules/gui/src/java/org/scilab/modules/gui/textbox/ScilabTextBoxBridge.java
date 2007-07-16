
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab TextBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabTextBoxBridge {

	/**
	 * Constructor
	 */
	protected ScilabTextBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab TextBox
	 * @return the created TextBox
	 */
	public static SimpleTextBox createTextBox() {
		return new SwingScilabTextBox();
	}
	
	/**
	 * Sets the Text of a Scilab TextBox
	 * @param textBox the TextBox we want to set the Text of
	 * @param newText the Text we want to set to the TextBox
	 */
	public static void setText(SimpleTextBox textBox, String newText) {
		textBox.setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab TextBox
	 * @param textBox the TextBox we want to get the Text of
	 * @return the text of the TextBox
	 */
	public static String getText(SimpleTextBox textBox) {
		return textBox.getText();
	}
	
	/**
	 * Draws a Scilab textBox
	 * @param textBox the textBox to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(SimpleTextBox textBox) {
		textBox.draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab TextBox
	 * @param textBox the textBox we want to get the dimensions of
	 * @return the size of the textBox
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(SimpleTextBox textBox) {
		return textBox.getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab textBox
	 * @param textBox the textBox we want to get the position of
	 * @return the position of the textBox
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(SimpleTextBox textBox) {
		return textBox.getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab TextBox
	 * @param textBox the textBox we want to get the visiblity status of
	 * @return the visibility status of the textBox (true if the textBox is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(SimpleTextBox textBox) {
		return textBox.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab TextBox
	 * @param textBox the textBox we want to set the dimensions of
	 * @param newSize the size we want to set to the textBox
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(SimpleTextBox textBox, Size newSize) {
		textBox.setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab textBox
	 * @param textBox the textBox we want to set the position of
	 * @param newPosition the position we want to set to the textBox
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(SimpleTextBox textBox, Position newPosition) {
		textBox.setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab TextBox
	 * @param textBox the textBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the textBox (true to set the textBox visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(SimpleTextBox textBox, boolean newVisibleState) {
		textBox.setVisible(newVisibleState);
	}
}
