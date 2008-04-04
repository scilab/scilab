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

package org.scilab.modules.gui.bridge.listbox;

import java.awt.Color;
import java.awt.Font;
import java.util.StringTokenizer;

import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.listbox.SimpleListBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab ListBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabListBox extends JScrollPane implements SimpleListBox {
	
	private static final long serialVersionUID = 3507396207331058895L;

	private CallBack callback;
	
	private ListSelectionListener listSelectionListener;
	
	/**
	 * the JList we use
	 */
	private JList list;
	
	/**
	 * Constructor
	 */
	public SwingScilabListBox() {
		super();
		getViewport().add(getList());
	}

	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		return getList().getBackground();
	}
	
	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		return getList().getFont();
	}
	
	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		return getList().getForeground();
	}
	
	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		getList().setBackground(color);
	}

	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	public void setFont(Font font) {
		getList().setFont(font);
	}
	
	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		getList().setForeground(color);
	}
	
	/**
	 * Draws a swing Scilab tab
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
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
		return new Position(getX(), getY() + getDims().getHeight() - (int) getHorizontalScrollBar().getPreferredSize().getHeight());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab tab
	 * @param newSize the dimensions we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		int absoluteY = getPosition().getY();
		setSize(newSize.getWidth(), newSize.getHeight());
		setPosition(new Position(getPosition().getX(), absoluteY));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		setLocation(newPosition.getX(), newPosition.getY() - getSize().height 
				+ (int) getHorizontalScrollBar().getPreferredSize().getHeight());
	}

	/**
	 * Sets the visibility status of an UIElement
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 *                      (true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
		list.setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the CheckBox
	 * @param cb the callback to set.
	 */
	public void setCallback(CallBack cb) {
		if (listSelectionListener != null) {
			getList().removeListSelectionListener(listSelectionListener);
		}
		this.callback = cb;
		
		listSelectionListener = new ListSelectionListener() {
			public void valueChanged(ListSelectionEvent arg0) {
				callback.actionPerformed(null);
			}
		};
		
		getList().addListSelectionListener(listSelectionListener);
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the first item text
	 * @return the items
	 * @see org.scilab.modules.gui.widget.Widget#getText()
	 */
	public String getText() {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the text of all the list items
	 * @return the items
	 * @see org.scilab.modules.gui.listbox.ListBox#getAllItemsText()
	 */
	public String[] getAllItemsText() {
		String[] retValue = new String[getList().getModel().getSize()];
		for (int i = 0; i < getList().getModel().getSize(); i++) {
			retValue[i] = (String) getList().getModel().getElementAt(i);
		}
		return retValue;
	}

	/**
	 * Get the number of items in the list
	 * @return the number of items
	 * @see org.scilab.modules.gui.listbox.ListBox#getNumberOfItems()
	 */
	public int getNumberOfItems() {
		return getList().getModel().getSize();
	}

	/**
	 * Set the text of the list items
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String text) {
		((DefaultListModel) getList().getModel()).clear();
		((DefaultListModel) getList().getModel()).addElement(text);
	}
	
	/**
	 * Set the text of the list items
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String[] text) {
		/* Clear previous items */
		((DefaultListModel) getList().getModel()).clear();
		
		/* Special case if the text contains | to separate items */
		if (text.length == 1) {
			StringTokenizer strTok = new StringTokenizer(text[0], "|");
			while (strTok.hasMoreTokens()) {
				((DefaultListModel) getList().getModel()).addElement(strTok.nextToken());
			}
		} else {
			for (int i = 0; i < text.length; i++) {
				((DefaultListModel) getList().getModel()).addElement(text[i]);
			}
		}
	}
	
	/**
	 * Set the horizontal alignment for the ListBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the vertical alignment for the ListBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set if more than one item can be selected in a ListBox
	 * @param status true if multiple selection is enabled
	 */
	public void setMultipleSelectionEnabled(boolean status) {
		if (status) {
			getList().setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		} else {
			getList().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		}
	}

	/**
	 * Set the selected indices of the ListBox
	 * @param indices the indices of the items to be selected
	 */
	public void setSelectedIndices(int[] indices) {
		// Scilab indices in Value begin at 1 and Java indices begin at 0
		int[] javaIndices = indices.clone();
		for (int i = 0; i < javaIndices.length; i++) {
			javaIndices[i] = javaIndices[i] - 1;
		}
		getList().setSelectedIndices(javaIndices);
	}
	
	/**
	 * Get the selected indices of the ListBox
	 * @return the indices of the items selected
	 */
	public int[] getSelectedIndices() {
		// Scilab indices in Value begin at 1 and Java indices begin at 0
		int[] javaIndices = getList().getSelectedIndices().clone();
		int[] scilabIndices = javaIndices.clone();
		for (int i = 0; i < getList().getSelectedIndices().length; i++) {
			scilabIndices[i] = scilabIndices[i] + 1;
		}
		return scilabIndices;
	}

	/**
	 * Get the number of items selected in the ListBox
	 * @return the number of items selected
	 */
	public int getSelectionSize() {
		return getList().getSelectedIndices().length;
	}
	
	/**
	 * Get or create the list component
	 * @return the list
	 */
	private JList getList() {
		if (list == null) {
			list = new JList();
			list.setLayoutOrientation(JList.VERTICAL);
			list.setModel(new DefaultListModel());
		}
		return list;
	}

	/**
	 * Set the Relief of the ListBox
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the ListBox
	 */
	public void destroy() {
		getParent().remove(this);
		this.setVisible(false);
	}
	
	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the ListBox.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the ListBox.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

}
