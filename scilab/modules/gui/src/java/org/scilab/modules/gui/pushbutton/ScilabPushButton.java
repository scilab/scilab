
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.utils.Position;
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

	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * If the maximum size has been set to a non-null value just returns it.
	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
	 * otherwise defer to the component's layout manager.
	 * @return the maximum size
	 */
	public Dimension getMaximumSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * If the minimum size has been set to a non-null value just returns it.
	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
	 * otherwise defer to the component's layout manager.
	 * @return the minimum size
	 */
	public Dimension getMinimumSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * If the preferredSize has been set to a non-null value just returns it. If the UI delegate's getPreferredSize
	 *  method returns a non null value then return that; otherwise defer to the component's layout manager.
	 * @return the preferredSize
	 */
	public Dimension getPreferredSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	public Dimension getSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * To get the x coordinate  of the element.
	 * @return the x coordinate
	 */
	public int getX() {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * To get the y coordinate  of the element.
	 * @return the y coordinate
	 */
	public int getY() {
		// TODO Auto-generated method stub
		return 0;
	}
	
	/**
	 * check if the PushButton is enable or disable.
	 * @return true if enable or false if disable
	 */
	public boolean isEnabled() {
		// TODO Auto-generated method stub
		return false;
	}

	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * To enable or disable the PushButton.
	 * @param value true to enable or false to disable
	 */
	public void setEnabled(boolean value) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	public void setFont(Font font) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	public void setLocation(int x, int y) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Sets the maximum size of the element.
	 * @param dimension the Dimension
	 */
	public void setMaximumSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Sets the minimum size of the element.
	 * @param dimension the Dimension
	 */
	public void setMinimumSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Sets the preferred size of the element.
	 * @param dimension the Dimension
	 */
	public void setPreferredSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	public void setSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Sets the text of a PushButton
	 * @param text the text to set to the PushButton
	 */
	public void setText(String text) {
		// TODO Auto-generated method stub
		
	}

}
