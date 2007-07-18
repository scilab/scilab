
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.textbox;

import javax.swing.JTextArea;

import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab TextBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTextBox extends JTextArea implements SimpleTextBox {

	/**
	 * Constructor
	 */
	public SwingScilabTextBox() {
		super();
		setEditable(false);
	}

	/**
	 * Sets the text of a Text Widget
	 * @param newText the text to set to the Text Widget
	 */
	@Override
	public void setText(String newText) {
		super.setText(newText);
	}

	/**
	 * Gets the text of a Text Widget
	 * @return the text of the Text Widget
	 */
	@Override
	public String getText() {
		return super.getText();
	}
	
	/**
	 * Draws a swing Scilab TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab TextBox
	 * @return the dimensions of the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab TextBox
	 * @return the position of the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab TextBox
	 * @param newSize the dimensions to set to the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab TextBox
	 * @param newPosition the position to set to the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	
}
