
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.editbox;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;

/**
 * Class for Scilab EditBoxs in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabEditBox extends ScilabWidget implements EditBox {

	private SimpleEditBox component;
	
	/**
	 * Constructor
	 */
	protected ScilabEditBox() {
		component = ScilabBridge.createEditBox();
	}

	/**
	 * Creates a Scilab EditBox object
	 * @return the created EditBox
	 */
	public static EditBox createEditBox() {
		return new ScilabEditBox();
	}
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleEditBox getAsSimpleEditBox() {
		return component;
	}
	
	/**
	 * Sets the text of a EditBox
	 * @param newText the text to set to the EditBox
	 */
	public void setText(String newText) {
		ScilabBridge.setText(this, newText);
	}
	
	/**
	 * Gets the text of a EditBox
	 * @return text of the EditBox
	 */
	public String getText() {
		return ScilabBridge.getText(this);
	}
	
	
//	TODO Question: What do we do with draw() for a EditBox ?
	/**
	 * Draws a EditBox
	 */
	public void draw() {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException(); 
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

//	TODO Question: What do we do with isVisible() for an EditBox ?
	/**
	 * Gets the visibility status of an UIElement
	 * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
	 */
	public boolean isVisible() {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//return ScilabBridge.isVisible(component);
		throw new UnsupportedOperationException(); 
	}
	
//	TODO Question: What do we do with setVisible() for an EditBox ?
	/**
	 * Sets the visibility status of a Menu
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 * 			(true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		//ScilabBridge.setVisible(component, newVisibleState);
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
	 * check if the EditBox is enable or disable.
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
	 * To enable or disable the EditBox.
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
	 * Sets the icon of a EditBox
	 * @param filename the path to the icon image to set to the EditBox
	 */
	public void setIcon(String filename) {
		// TODO Auto-generated method stub
		
	}

	// TODO this will be deleted
	/**
	 * Add this as member (dockable element) to the Scilab Container and returns its index
	 * @param container the Container in which we add this
	 * @return index of this in container components
	 */
	public int addAsMemberTo(Container container) {
//		 TODO code this
		//return ScilabBridge.addAsMemberTo(this, container);
		return 0;
	}
}
