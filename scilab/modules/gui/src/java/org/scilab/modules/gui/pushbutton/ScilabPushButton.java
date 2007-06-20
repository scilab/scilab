package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.utils.Position;

/* Copyright INRIA 2007 */

import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;
import org.scilab.modules.gui.bridge.ScilabBridge;

/**
* Class for Scilab PushButtons in GUIs
* @author Marouane BEN JELLOUL
*/
public class ScilabPushButton extends ScilabWidget implements PushButton {

	/**
	 * Constructor
	 */
	protected ScilabPushButton() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab PushButton object
	 * @return the created PushButton
	 */
	public static PushButton createPushButton() {
		return ScilabBridge.createPushButton();
	}
	
//	TODO Question: What do we do with draw() for a PushButton ?
	/**
	 * Draws a PushButton
	 */
	public void draw() {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException(); 
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab PushButton
	 * @param newSize the dimensions to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//ScilabBridge.setDims(this, newSize);
		throw new UnsupportedOperationException(); 
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab Menu
	 * @return the dimensions (width and height) of the window
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims(org.scilab.modules.gui.utils.Size)
	 */
	public Size getDims() {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//return ScilabBridge.getDims(this);
		throw new UnsupportedOperationException(); 
	}
	
	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab Menu
	 * @return the position of the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//return ScilabBridge.getPosition(this);
		throw new UnsupportedOperationException(); 
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab Menu
	 * @param newPosition the position we want to set to the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//ScilabBridge.setPosition(this, newPosition);
		throw new UnsupportedOperationException(); 
	}

//	TODO Question: What do we do with isVisible() for a Menu ?
	/**
	 * Gets the visibility status of an UIElement
	 * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
	 */
	public boolean isVisible() {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//return ScilabBridge.isVisible(this);
		throw new UnsupportedOperationException(); 
	}
	
//	TODO Question: What do we do with setVisible() for a Menu ?
	/**
	 * Sets the visibility status of a Menu
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 * 			(true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//ScilabBridge.setVisible(this, newVisibleState);
		throw new UnsupportedOperationException();
	}

}
