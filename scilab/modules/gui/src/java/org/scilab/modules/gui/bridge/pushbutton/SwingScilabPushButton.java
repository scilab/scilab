
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.pushbutton;

import java.awt.Color;

import javax.swing.JButton;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
* Swing implementation for Scilab PushButtons in GUIs
* @author Marouane BEN JELLOUL
*/
public class SwingScilabPushButton extends JButton implements PushButton {

	/**
	 * Constructor
	 */
	public SwingScilabPushButton() {
		super();
	}

	// TODO Question: What do we do with draw() for a PushButton ?
	/**
	 * 
	 */
	public void draw() {
		// TODO Auto-generated method stub
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab PushButton
	 * @return the dimensions of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @return the position of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab PushButton
	 * @param newSize the dimensions to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @param newPosition the position to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

//	/**
//	 * Sets the text of a swing Scilab PushButton
//	 * @param text the text to set to the PushButton
//	*/
//	public void setText(String text) {
//		super.setText(text);
//	}
//	
//	/**
//	 * To set the Background color of the element.
//	 * @param color the Color
//	 */
//	public void setBackground(Color color) {
//		super.setBackground(color);
//	}
//	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
//	
//	/**
//	 * To enable or disable the PushButton.
//	 * @param value true to enable or false to disable
//	 */
//	public void setEnabled(boolean value) {
//		super.setEnabled(value);
//	}
}

