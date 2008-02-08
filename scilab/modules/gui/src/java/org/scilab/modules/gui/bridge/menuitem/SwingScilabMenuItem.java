
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menuitem;

import javax.swing.JMenu;
import javax.swing.JMenuItem;

import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.SimpleMenuItem;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuItem extends JMenuItem implements SimpleMenuItem {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public SwingScilabMenuItem() {
		super();
	}
	
	/**
	 * Add a callback to the MenuItem, this callback is a Scilab command
	 * @param command the Scilab command to execute when the MenuItem is activated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		addActionListener(ScilabCallBack.create(command, commandType));
	}

	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic to add to the MenuItem
	 * @see org.scilab.modules.gui.widget.MenuItem#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(int mnemonic) {
		super.setMnemonic(mnemonic); 	 
	}
	
	/**
	 * Add a Scilab MenuItemBar to a Scilab MenuItem
	 * @param menuBarToAdd the Scilab MenuBar to add to the Scilab MenuItem
	 * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Add a Scilab Toolbar to a Scilab MenuItem
	 * @param toolBarToAdd the Scilab ToolBar to add to the Scilab MenuItem
	 * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.menubar.ToolBar)
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Draws a swing Scilab MenuItem
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab MenuItem
	 * @return the dimensions of the menu
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab MenuItem
	 * @return the position of the menu
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab MenuItem
	 * @param newSize the dimensions to set to the menu
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab MenuItem
	 * @param newPosition the position to set to the menu
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the id of the menu bar associated to the menu (not the parent menubar)
	 * @return the menubar
	 * @see org.scilab.modules.gui.uielement.UIElement#getMenuBarId()
	 */
	public MenuBar getMenuBar() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the id of the tool bar associated to the menu
	 * @return the toolbar
	 * @see org.scilab.modules.gui.uielement.UIElement#getToolBarId()
	 */
	public ToolBar getToolBar() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the Menu text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the Menu text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}
	
	/**
	 * Set the Relief of the Menu
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Add a MenuItem to this MenuItem
	 * @param childMenuItem the MenuItem we want to add
	 */
	public void add(MenuItem childMenuItem) {
		int index = getParent().getComponentZOrder(this);
		Menu newMenu = ScilabMenu.createMenu();
		newMenu.setText(getText());
		destroy();
		((JMenu) getParent()).add((JMenu) newMenu.getAsSimpleMenu());
	}

	/**
	 * Destroy the MenuItem
	 */
	public void destroy() {
		getParent().remove(this);
	}

}
