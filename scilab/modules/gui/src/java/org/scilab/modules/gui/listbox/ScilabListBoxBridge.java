/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.listbox;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab ListBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabListBoxBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabListBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab ListBox
	 * @return the created ListBox
	 */
	public static SimpleListBox createListBox() {
		return new SwingScilabListBox();
	}
	
	/**
	 * Sets the Text of a Scilab ListBox
	 * @param listBox the ListBox we want to set the Text of
	 * @param newText the Text we want to set to the ListBox
	 */
	public static void setText(ListBox listBox, String newText) {
		listBox.getAsSimpleListBox().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab ListBox
	 * @param listBox the ListBox we want to get the Text of
	 * @return the text of the ListBox
	 */
	public static String getText(ListBox listBox) {
		return listBox.getAsSimpleListBox().getText();
	}
	
	/**
	 * Draws a Scilab listBox
	 * @param listBox the listBox to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(ListBox listBox) {
		listBox.getAsSimpleListBox().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab ListBox
	 * @param listBox the listBox we want to get the dimensions of
	 * @return the size of the listBox
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(ListBox listBox) {
		return listBox.getAsSimpleListBox().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab listBox
	 * @param listBox the listBox we want to get the position of
	 * @return the position of the listBox
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(ListBox listBox) {
		return listBox.getAsSimpleListBox().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab ListBox
	 * @param listBox the listBox we want to get the visiblity status of
	 * @return the visibility status of the listBox (true if the listBox is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(ListBox listBox) {
		return listBox.getAsSimpleListBox().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab ListBox
	 * @param listBox the listBox we want to set the dimensions of
	 * @param newSize the size we want to set to the listBox
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(ListBox listBox, Size newSize) {
		listBox.getAsSimpleListBox().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab listBox
	 * @param listBox the listBox we want to set the position of
	 * @param newPosition the position we want to set to the listBox
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(ListBox listBox, Position newPosition) {
		listBox.getAsSimpleListBox().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab ListBox
	 * @param listBox the listBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the listBox (true to set the listBox visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(ListBox listBox, boolean newVisibleState) {
		listBox.getAsSimpleListBox().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the ListBox
	 * @param listBox the ListBox we want to set the callback of
	 * @param callback the callback to set.
	 */
	public static void setCallback(ListBox listBox, CallBack callback) {
		listBox.getAsSimpleListBox().setCallback(callback);
	}
	
	/**
	 * To set the Background color of the ListBox
	 * @param listBox the ListBox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(ListBox listBox, Color color) {
		listBox.getAsSimpleListBox().setBackground(color);
	}

	/**
	 * Get the Background color of the ListBox
	 * @param listBox the ListBox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(ListBox listBox) {
		return listBox.getAsSimpleListBox().getBackground();
	}

	/**
	 * To set the Foreground color of the listBox
	 * @param listBox the ListBox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(ListBox listBox, Color color) {
		listBox.getAsSimpleListBox().setForeground(color);
	}

	/**
	 * Get the Foreground color of the ListBox
	 * @param listBox the ListBox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(ListBox listBox) {
		return listBox.getAsSimpleListBox().getForeground();
	}

	/**
	 * Set the font of the ListBox.
	 * @param listBox the ListBox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(ListBox listBox, Font font) {
		listBox.getAsSimpleListBox().setFont(font);
	}
	
	/**
	 * Get the font of the ListBox.
	 * @param listBox the ListBox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(ListBox listBox) {
		return listBox.getAsSimpleListBox().getFont();
	}
	
	/**
	 * Set if the ListBox is enabled or not
	 * @param listBox the ListBox we want to set the status of
	 * @param status true if the ListBox is enabled
	 */
	public static void setEnabled(ListBox listBox, boolean status) {
		listBox.getAsSimpleListBox().setEnabled(status);
	}
	
	/**
	 * Gets the enable status of a ListBox
	 * @param listBox the ListBox we want to get the status of
	 * @return the enable status of the ListBox (true if the ListBox is enabled, false if not)
	 */
	public static boolean isEnabled(ListBox listBox) {
		return listBox.getAsSimpleListBox().isEnabled();
	}
	
	/**
	 * Set the horizontal alignment for the ListBox text
	 * @param listBox the ListBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(ListBox listBox, String alignment) {
		listBox.getAsSimpleListBox().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the ListBox text
	 * @param listBox the ListBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(ListBox listBox, String alignment) {
		listBox.getAsSimpleListBox().setVerticalAlignment(alignment);
	}

	/**
	 * Set if more than one item can be selected in a ListBox
	 * @param listBox the ListBox we want to set the status of
	 * @param status true if multiple selection is enabled
	 */
	public static void setMultipleSelectionEnabled(ListBox listBox, boolean status) {
		listBox.getAsSimpleListBox().setMultipleSelectionEnabled(status);
	}

	/**
	 * Set the selected indices of the ListBox
	 * @param listBox the ListBox we want to set the selected indices of
	 * @param indices the indices of the items to be selected
	 */
	public static void setSelectedIndices(ListBox listBox, int[] indices) {
		listBox.getAsSimpleListBox().setSelectedIndices(indices);
	}
	
	/**
	 * Get the selected indices of the ListBox
	 * @param listBox the ListBox we want to get the seleted indices of
	 * @return the indices of the items selected
	 */
	public static int[] getSelectedIndices(ListBox listBox) {
		return listBox.getAsSimpleListBox().getSelectedIndices();
	}
	
	/**
	 * Get the number of items selected in the ListBox
	 * @param listBox the ListBox we want to get the selection size of
	 * @return the number of items selected
	 */
	public static int getSelectionSize(ListBox listBox) {
		return listBox.getAsSimpleListBox().getSelectionSize();
	}
	
	/**
	 * Get the text of all the list items
	 * @param listBox the ListBox we want to get the items text of
	 * @return the text items
	 * @see org.scilab.modules.gui.listbox.ListBox#getAllItemsText()
	 */
	public static String[] getAllItemsText(ListBox listBox) {
		return listBox.getAsSimpleListBox().getAllItemsText();
	}

	/**
	 * Get the number of items in the list
	 * @param listBox the ListBox we want to get the items number of
	 * @return the number of items
	 * @see org.scilab.modules.gui.listbox.ListBox#getNumberOfItems()
	 */
	public static int getNumberOfItems(ListBox listBox) {
		return listBox.getAsSimpleListBox().getNumberOfItems();
	}

	/**
	 * Set the text of the list items
	 * @param listBox the ListBox we want to set the items text of
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.listbox.ListBox#setText(java.lang.String[])
	 */
	public static void setText(ListBox listBox, String[] text) {
		listBox.getAsSimpleListBox().setText(text);
	}
	
	/**
	 * Set the Relief of the ListBox
	 * @param listBox the ListBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(ListBox listBox, String reliefType) {
		listBox.getAsSimpleListBox().setRelief(reliefType);
	}

	/**
	 * Delete the ListBox
	 * @param listBox the ListBox to be destroyed
	 */
	public static void destroy(ListBox listBox) {
		listBox.getAsSimpleListBox().destroy();
	}
	
	/**
	 * Give the focus to the ListBox
	 * @param listBox the ListBox
	 */
	public static void requestFocus(ListBox listBox) {
		listBox.getAsSimpleListBox().requestFocus();
	}
	
	/**
	 * Adjusts the view so that the element given by index is displayed at the top of the ListBox.
	 * @param listBox the ListBox
	 * @param index the index of the element to be displayed at the top of the ListBox.
	 */
	public static void setListBoxTop(ListBox listBox, int index) {
		listBox.getAsSimpleListBox().setListBoxTop(index);
	}
	
	/**
	 * Gets the index of the element displayed at the top of the ListBox
	 * @param listBox the ListBox
	 * @return the index of the element displayed at the top of the ListBox
	 */
	public static int getListBoxTop(ListBox listBox) {
		return listBox.getAsSimpleListBox().getListBoxTop();
	}

}
