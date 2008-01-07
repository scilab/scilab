
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menu;

import javax.swing.JMenu;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciActionListener;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenu extends JMenu implements SimpleMenu {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public SwingScilabMenu() {
		super();
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public void add(MenuItem newMenuItem) {
		super.add((SwingScilabMenuItem) newMenuItem.getAsSimpleMenuItem());
	}
	
	/**
	 * Append a subMenu to a Scilab Menu
	 * @param newSubMenu the subMenu to append to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.Menu)
	 */
	public void add(Menu newSubMenu) {
		super.add((SwingScilabMenu) newSubMenu.getAsSimpleMenu());
	}
	
	/**
	 * Sets the Text of a swing Scilab Menu
	 * @param newText the Text to set to the Menu
	 * @see javax.swing.AbstractButton#setText(java.lang.String)
	 */
	@Override
	public void setText(String newText) {
		super.setText(newText);
	}
	
	/**
	 * Get the Text of a swing Scilab Menu
	 * @return the label of the Menu
	 * @see javax.swing.AbstractButton#setText(java.lang.String)
	 */
	@Override
	public String getText() {
		return super.getText();
	}

	/**
	 * set a mnemonic to a Menu
	 * @param mnemonic the mnemonic to set to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	@Override
	public void setMnemonic(int mnemonic) {
		super.setMnemonic(mnemonic);
	}

	/**
	 * Add a Separator to a Menu
	 * @see org.scilab.modules.gui.menu.Menu#addSeparator(org.scilab.modules.gui..)
	 */
	@Override
	public void addSeparator() {
		// TODO correct this it should be something like super.add(SwingScilabSeparator) ... ???
		super.addSeparator();
	}

	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 */
	public void setCallback(String command) {
		super.addMenuListener(new SciActionListener(command));
	}

	/**
	 * Add a Scilab MenuBar to a Scilab menu
	 * @param menuBarToAdd the Scilab MenuBar to add to the Scilab menu
	 * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Add a Scilab Toolbar to a Scilab menu
	 * @param toolBarToAdd the Scilab ToolBar to add to the Scilab menu
	 * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.menubar.ToolBar)
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Draws a swing Scilab menu
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab menu
	 * @return the dimensions of the menu
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab menu
	 * @return the position of the menu
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab menu
	 * @param newSize the dimensions to set to the menu
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab menu
	 * @param newPosition the position to set to the menu
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Get the id of the menu bar associated to the menu (not the parent menubar)
	 * @return the menubar
	 * @see org.scilab.modules.gui.uielement.UIElement#getMenuBarId()
	 */
	public MenuBar getMenuBar() {
		// Must not be there...
		return null;
	}

	/**
	 * Get the id of the tool bar associated to the menu
	 * @return the toolbar
	 * @see org.scilab.modules.gui.uielement.UIElement#getToolBarId()
	 */
	public ToolBar getToolBar() {
		// Must not be there...
		return null;
	}

	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		addActionListener(ScilabCallBack.create(command, commandType));
	}
}
