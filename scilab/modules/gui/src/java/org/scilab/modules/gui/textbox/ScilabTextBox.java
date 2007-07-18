
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.text.ScilabText;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab EditBoxs in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabTextBox extends ScilabText implements TextBox {
	
	private SimpleTextBox component;
	
	/**
	 * Constructor
	 */
	protected ScilabTextBox() {
		component = ScilabBridge.createTextBox();
	}

	/**
	 * Creates a Scilab TextBox object
	 * @return the created TextBox
	 */
	public static TextBox createTextBox() {
		return new ScilabTextBox();
	}
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleTextBox getAsSimpleTextBox() {
		return component;
	}
	
	/**
	 * Sets the text of a TextBox
	 * @param newText the text to set to the TextBox
	 */
	public void setText(String newText) {
		ScilabBridge.setText(this, newText);
	}
	
	/**
	 * Gets the text of a TextBox
	 * @return text of the TextBox
	 */
	public String getText() {
		return ScilabBridge.getText(this);
	}
	
	/**
	 * Draws a TextBox
	 */
	public void draw() {
		ScilabBridge.draw(this);
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab element
	 * @param newSize the dimensions to set to the element
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(this, newSize); 
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab element
	 * @return the dimensions (width and height) of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims(org.scilab.modules.gui.utils.Size)
	 */
	public Size getDims() {
		return ScilabBridge.getDims(this); 
	}
	
	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab element
	 * @return the position of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(this); 
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab element
	 * @param newPosition the position we want to set to the element
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(this, newPosition);
	}

	/**
	 * Gets the visibility status of an element
	 * @return the visibility status of the element (true if the element is visible, false if not)
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(this); 
	}
	
	/**
	 * Sets the visibility status of a element
	 * @param newVisibleState the visibility status we want to set for the element
	 * 			(true if the element is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(this, newVisibleState);
	}
	
}
