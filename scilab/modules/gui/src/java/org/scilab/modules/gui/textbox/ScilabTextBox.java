
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.text.ScilabText;
import org.scilab.modules.gui.toolbar.ToolBar;
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
	 * Sets a MenuBar to an element
	 * @param newMenuBar the MenuBar to set to the element
	 */
	public void addMenuBar(MenuBar newMenuBar) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Sets a ToolBar to an element
	 * @param newToolBar the ToolBar to set to the element
	 */
	public void addToolBar(ToolBar newToolBar) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Sets the text of a TextBox
	 * @param newText the text to set to the TextBox
	 */
	public void setText(String newText) {
		ScilabBridge.setText(component, newText);
	}
	
	/**
	 * Gets the text of a TextBox
	 * @return text of the TextBox
	 */
	public String getText() {
		return ScilabBridge.getText(component);
	}
	
	
//	TODO Question: What do we do with draw() for a TextBox ?
	/**
	 * Draws a TextBox
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
		ScilabBridge.setDims(component, newSize); 
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab element
	 * @return the dimensions (width and height) of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims(org.scilab.modules.gui.utils.Size)
	 */
	public Size getDims() {
		return ScilabBridge.getDims(component); 
	}
	
	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab element
	 * @return the position of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(component); 
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab element
	 * @param newPosition the position we want to set to the element
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(component, newPosition);
	}

//	TODO Question: What do we do with isVisible() for an TextBox ?
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
	
//	TODO Question: What do we do with setVisible() for an TextBox ?
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
