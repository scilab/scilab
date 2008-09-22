/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.popupmenu;

import java.util.StringTokenizer;

import javax.swing.JComboBox;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.popupmenu.SimplePopupMenu;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab PopupMenu in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPopupMenu extends JComboBox implements SimplePopupMenu {

	private static final long serialVersionUID = -4366581303317502544L;

	private CallBack callback;
	
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
		return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab tab
	 * @param newSize the dimensions we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		setSize(newSize.getWidth(), newSize.getHeight());
		doLayout(); /* Needed !! because PopupMenu is badly drawn else */
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}

	/**
	 * Add a callback to the PopupMenu
	 * @param callback the callback to set.
	 */
	public void setCallback(CallBack callback) {
		if (this.callback != null) {
			removeActionListener(this.callback);
		}
		this.callback = callback;
		addActionListener(this.callback);
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
	public void setUserSelectedIndex(int index) {
		/* Remove the listener to avoid the callback to be executed */
		if (this.callback != null) {
			removeActionListener(this.callback);
		}

		for (int i = 0; i < getItemCount(); i++) {
			// Scilab indices in Value begin at 1 and Java indices begin at 0
			if (i == (index - 1)) {
				getModel().setSelectedItem(getItemAt(i));
			}
		}
		
		/* Put back the listener */
		if (this.callback != null) {
			addActionListener(this.callback);
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
				// Scilab indices in Value begin at 1 and Java indices begin at 0
				return i + 1;
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
			retValue[i] = getItemAt(i).toString();
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
		/* Remove the listener to avoid the callback to be executed */
		if (this.callback != null) {
			removeActionListener(this.callback);
		}

		/* Clear previous items */
		removeAllItems();
		
		/* Special case if the text contains | to separate items */
		if (text.length == 1) {
			if (text[0].length() == 0) {
				/* Clear the popup items */
				return;
			}
			StringTokenizer strTok = new StringTokenizer(text[0], "|");
			while (strTok.hasMoreTokens()) {
				addItem(strTok.nextToken());
			}
		} else {
			for (int i = 0; i < text.length; i++) {
				addItem(text[i]);
			}
		}

		/* Remove the listener to avoid the callback to be executed */
		if (this.callback != null) {
			addActionListener(this.callback);
		}

	}

	/**
	 * Set the Relief of the PopupMenu
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the PopupMenu
	 */
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}
	
	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the PopupMenu.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the PopupMenu.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for PopupMenus */
		throw new UnsupportedOperationException();
	}

}
