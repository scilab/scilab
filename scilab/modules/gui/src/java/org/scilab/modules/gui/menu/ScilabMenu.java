
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenu extends ScilabWidget implements Menu {

	/**
	 * Constructor
	 */
	protected ScilabMenu() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Menu object
	 * @return the created Menu
	 */
	public static Menu createMenu() {
		return ScilabBridge.createMenu();
	}
	
//	TODO Question: What do we do with draw() for a Menu ?
	/**
	 * Draws a Menu
	 */
	public void draw() {
		// TODO Auto-generated method stub
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab Menu
	 * @param newSize the dimensions to set to the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(this, newSize);
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab Menu
	 * @return the dimensions (width and height) of the window
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims(org.scilab.modules.gui.utils.Size)
	 */
	public Size getDims() {
		return ScilabBridge.getDims(this);
	}
	
	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab Menu
	 * @return the position of the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(this);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab Menu
	 * @param newPosition the position we want to set to the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(this, newPosition);
	}

//	TODO Question: What do we do with isVisible() for a Menu ?
	/**
	 * Gets the visibility status of an UIElement
	 * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(this);
	}
	
//	TODO Question: What do we do with setVisible() for a Menu ?
	/**
	 * Sets the visibility status of a Menu
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 * 			(true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(this, newVisibleState);
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public MenuItem add(MenuItem newMenuItem) {
		return ScilabBridge.add(this, newMenuItem);
	}
	
	/**
	 * Sets the text of a Scilab Menu
	 * @param newText the Text to set to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setText(java.lang.String)
	 */
	public void setText(String newText) {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		// ScilabBridge.setText(this, newText);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param mnemonic the mnemonic to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(int mnemonic) {
		ScilabBridge.setMnemonic(this, mnemonic);
	}
	
	/**
	 * Add a Separator to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#addSeparator(org.scilab.modules.gui..)
	 */
	public void addSeparator() {
		ScilabBridge.addSeparator(this);
	}
}
