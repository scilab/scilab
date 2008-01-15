
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.popupmenu;

import javax.swing.JComboBox;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.popupmenu.SimplePopupMenu;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab PopupMenu in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPopupMenu extends JComboBox implements SimplePopupMenu {

	private static final long serialVersionUID = -4366581303317502544L;

	/**
	 * Constructor
	 */
	public SwingScilabPopupMenu() {
		super();
	}
	
	/**
	 * Draws a swing Scilab tab
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab tab
	 * @return the dimensions of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(getWidth(), getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(getX(), getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab tab
	 * @param newSize the dimensions we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Add a callback to the PopupMenu
	 * @param command the Scilab command to execute when the PopupMenu is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		System.out.println("setCallback(String command, int commandType) is not yet implemented for SwingScilabPopupMenu");
		//addActionListener(ScilabCallBack.create(command, commandType));
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Get the text if the PopupMenu items
	 * @return the items
	 * @see org.scilab.modules.gui.widget.Widget#getText()
	 */
	public String getText() {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the text of the PopupMenu items
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String text) {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the PopupMenu text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
		
	}

	/**
	 * Set the vertical alignment for the PopupMenu text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the selected index of the PopupMenu
	 * @param index the index of the item to be selected
	 */
	public void setSelectedIndex(int index) {
		for (int i = 0; i < getItemCount(); i++) {
			if (i == index) {
				getModel().setSelectedItem(getItemAt(i));
			}
		}
	}
	
	/**
	 * Get the selected index of the PopupMenu
	 * @return the index of the item selected
	 */
	public int getSelectedIndex() {
		Object itemSelected = getModel().getSelectedItem();
		for (int i = 0; i < getItemCount(); i++) {
			if (itemSelected.equals(getItemAt(i))) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * Get the text of all the PopupMenu items
	 * @return the text items
	 */
	public String[] getAllItemsText() {
		String[] retValue = new String[getItemCount()];
		for (int i = 0; i < getItemCount(); i++) {
			retValue[i] = (String) getItemAt(i);
		}
		return retValue;
		
	}

	/**
	 * Get the number of items in the PopupMenu
	 * @return the number of items
	 */
	public int getNumberOfItems() {
		return getItemCount();
	}

	/**
	 * Set the text of the PopupMenu items
	 * @param text the text of the items
	 */
	public void setText(String[] text) {
		removeAllItems();
		for (int i = 0; i < text.length; i++) {
			addItem(text[i]);
		}
		
	}

}
