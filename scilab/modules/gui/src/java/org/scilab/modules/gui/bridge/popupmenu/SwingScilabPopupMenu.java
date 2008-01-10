
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.popupmenu;

import javax.swing.JPopupMenu;

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
public class SwingScilabPopupMenu extends JPopupMenu implements SimplePopupMenu {

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
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Set the text of the PopupMenu items
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String text) {
		// TODO Auto-generated method stub
	}

}
