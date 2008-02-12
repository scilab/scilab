
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.bridge.contextmenu;

import java.awt.MouseInfo;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;

import javax.swing.JPopupMenu;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.contextmenu.SimpleContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Menus in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabContextMenu extends JPopupMenu implements SimpleContextMenu {

	private static final long serialVersionUID = 1L;

	private CallBack callback;
	
	private MouseListener customedMouseListener;
	
	private String result = new String();
	
	/**
	 * Constructor
	 */
	public SwingScilabContextMenu() {
		super();
		setInvoker(this);
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public void add(MenuItem newMenuItem) {
		((SwingScilabMenuItem) newMenuItem.getAsSimpleMenuItem()).
				addActionListener(new ContextMenuItemActionListener(newMenuItem.getText()));
		super.add((SwingScilabMenuItem) newMenuItem.getAsSimpleMenuItem());
	}
	
	/**
	 * Display the ContextMenu
	 * @return the label of the menu selected
	 */
	public String display() {
		// Default location is at mouse pointer position
		setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
		setVisible(true);
		try {
			synchronized (this) {
				this.wait();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return result;
	}

	/**
	 * Add a Scilab MenuBar to a Scilab menu
	 * @param menuBarToAdd the Scilab MenuBar to add to the Scilab menu
	 * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Add a Scilab Toolbar to a Scilab menu
	 * @param toolBarToAdd the Scilab ToolBar to add to the Scilab menu
	 * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.menubar.ToolBar)
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Draws a swing Scilab menu
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab menu
	 * @return the dimensions of the menu
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab menu
	 * @return the position of the menu
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab menu
	 * @param newSize the dimensions to set to the menu
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab menu
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
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the Menu text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the vertical alignment for the Menu text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the Relief of the Menu
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the Menu
	 * @see org.scilab.modules.gui.menuitem.SimpleMenuItem#destroy()
	 */
	public void destroy() {
		getParent().remove(this);
	}

	/**
	 * Set if the menu is enabled or not
	 * @param status true if the menu is enabled
	 */
	public void setEnabled(boolean status) {
		super.setEnabled(status);
		/* (Des)Activate the callback */ 
		if (callback != null) {
			if (status) {
				addMouseListener(customedMouseListener);
			} else {
				removeMouseListener(customedMouseListener);
			}
		}
	}

	/**
	 * Not used
	 * @param mnemonic not used
	 */
	public void setMnemonic(char mnemonic) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @param mnemonic not used
	 */
	public void setMnemonic(int mnemonic) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @return null
	 */
	public String getText() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @param text not used
	 */
	public void setText(String text) {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the return value
	 * @param value the value to set
	 */
	public void setResult(String value) {
		result = value;
		synchronized (this) {
			this.notify();
		}
	}
	
	/**
	 * Class used to managed callback on menu items
	 * @author Vincent COUVERT
	 */
	private class ContextMenuItemActionListener implements ActionListener {

		private String label;
		
		/**
		 * Constructor
		 * @param itemLabel the label of the menu item
		 */
		public ContextMenuItemActionListener(String itemLabel) {
			label = itemLabel;	
		}

		/**
		 * Action performed ? What do I have to do ?
		 * @param arg0 the action
		 */
		public void actionPerformed(ActionEvent arg0) {
			setResult(label);
		}
		
	}

	
	/**
	 * Add a Menu to this MenuItem
	 * @param childMenu the Menu we want to add
	 */
	public void add(Menu childMenu) {
		// TODO Auto-generated method stub
		
	}
}
