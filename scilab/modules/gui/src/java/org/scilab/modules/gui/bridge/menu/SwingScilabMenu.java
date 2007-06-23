
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menu;

import javax.swing.JMenu;
import javax.swing.JMenuItem;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenu extends JMenu implements Menu {

	/**
	 * Constructor
	 */
	public SwingScilabMenu() {
		super();
	}
	
	// TODO Question: What do we do with draw() for a Menu ?
	/**
	 * 
	 */
	public void draw() {
		// TODO Auto-generated method stub
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab Menu
	 * @return the dimensions of the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab Menu
	 * @return the position of the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab Menu
	 * @param newSize the dimensions to set to the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab Menu
	 * @param newPosition the position to set to the Menu
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public MenuItem add(MenuItem newMenuItem) {
		return (MenuItem) super.add((JMenuItem) newMenuItem);
	}
	
	/**
	 * Sets the Text of a swing Scilab Menu
	 * @param newText the Text to set to the Menu
	 * @see javax.swing.AbstractButton#setText(java.lang.String)
	 */
	public void setText(String newText) {
		super.setText(newText);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param mnemonic the mnemonic to set to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(int mnemonic) {
		super.setMnemonic(mnemonic);
	}

	/**
	 * Add a Separator to a Menu
	 * @see org.scilab.modules.gui.menu.Menu#addSeparator(org.scilab.modules.gui..)
	 */
	public void addSeparator() {
		super.addSeparator();
	}
}
