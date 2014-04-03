/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.listbox;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;

/**
 * Class for Scilab EditBoxes in GUIs
 * @author Vincent COUVERT
 */
public class ScilabListBox extends ScilabWidget implements ListBox {

    private SimpleListBox component;

    /**
     * Constructor
     */
    protected ScilabListBox() {
        component = ScilabBridge.createListBox();
    }

    /**
     * Creates a Scilab ListBox object
     * @return the created ListBox
     */
    public static ListBox createListBox() {
        return new ScilabListBox();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleListBox getAsSimpleListBox() {
        return component;
    }

    /**
     * Sets the text of a ListBox
     * @param newText the text to set to the ListBox
     */
    public void setText(String newText) {
        ScilabBridge.setText(this, newText);
    }

    public void setEmptyText() {
        ScilabBridge.setText(this, new String[] {});
    }

    /**
      * Gets the text of a ListBox
      * @return text of the ListBox
      */
    public String getText() {
        return ScilabBridge.getText(this);
    }

    /**
     * Draws a ListBox
     */
    public void draw() {
        ScilabBridge.draw(this);
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab element
     * @param newSize the dimensions to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        ScilabBridge.setDims(this, newSize);
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab element
     * @return the dimensions (width and height) of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getDims(org.scilab.modules.gui.utils.Size)
     */
    public Size getDims() {
        return ScilabBridge.getDims(this);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab element
     * @return the position of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return ScilabBridge.getPosition(this);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab element
     * @param newPosition the position we want to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        ScilabBridge.setPosition(this, newPosition);
    }

    /**
     * Gets the visibility status of an UIElement
     * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
     */
    public boolean isVisible() {
        return ScilabBridge.isVisible(this);
    }

    /**
     * Sets the visibility status of a Menu
     * @param newVisibleState the visibility status we want to set for the UIElement
     * 			(true if the UIElement is visible, false if not)
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }

    /**
     * To get the Background color of the element.
     * @return color the Color
     */
    public Color getBackground() {
        return ScilabBridge.getBackground(this);
    }

    /**
     * To get the Font of the element.
     * @return font the Font
     */
    public Font getFont() {
        return ScilabBridge.getFont(this);
    }

    /**
     * To get the Foreground color of the element.
     * @return color the Color
     */
    public Color getForeground() {
        return ScilabBridge.getForeground(this);
    }

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    public void setBackground(Color color) {
        ScilabBridge.setBackground(this, color);
    }

    /**
     * To set the Font of the element.
     * @param font the Font
     */
    public void setFont(Font font) {
        ScilabBridge.setFont(this, font);
    }

    /**
     * To set the Foreground color of the element.
     * @param color the Color
     */
    public void setForeground(Color color) {
        ScilabBridge.setForeground(this, color);
    }

    /**
     * Add a callback to the ListBox
     * @param callback the CallBack to set.
     */
    public void setCallback(CommonCallBack callback) {
        ScilabBridge.setCallback(this, callback);
    }

    /**
     * Set if the ListBox is enabled or not
     * @param status true if the ListBox is enabled
     */
    public void setEnabled(boolean status) {
        ScilabBridge.setEnabled(this, status);
    }

    /**
     * Gets the enable status of a ListBox
     * @return the enable status of the ListBox (true if the ListBox is enabled, false if not)
     */
    public boolean isEnabled() {
        return ScilabBridge.isEnabled(this);
    }

    /**
     * Set the horizontal alignment for the ListBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        ScilabBridge.setHorizontalAlignment(this, alignment);
    }

    /**
     * Set the vertical alignment for the ListBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        ScilabBridge.setVerticalAlignment(this, alignment);
    }

    /**
     * Set if more than one item can be selected in a ListBox
     * @param status true if multiple selection is enabled
     */
    public void setMultipleSelectionEnabled(boolean status) {
        ScilabBridge.setMultipleSelectionEnabled(this, status);
    }

    /**
     * Set the selected indices of the ListBox
     * @param indices the indices of the items to be selected
     */
    public void setSelectedIndices(int[] indices) {
        ScilabBridge.setSelectedIndices(this, indices);
    }

    /**
     * Get the selected indices of the ListBox
     * @return the indices of the items selected
     */
    public int[] getSelectedIndices() {
        return ScilabBridge.getSelectedIndices(this);
    }

    /**
     * Get the number of items selected in the ListBox
     * @return the number of items selected
     */
    public int getSelectionSize() {
        return ScilabBridge.getSelectionSize(this);
    }

    /**
     * Get the text of all the list items
     * @return the text items
     * @see org.scilab.modules.gui.listbox.ListBox#getAllItemsText()
     */
    public String[] getAllItemsText() {
        return ScilabBridge.getAllItemsText(this);
    }

    /**
     * Get the number of items in the list
     * @return the number of items
     * @see org.scilab.modules.gui.listbox.ListBox#getNumberOfItems()
     */
    public int getNumberOfItems() {
        return ScilabBridge.getNumberOfItems(this);
    }

    /**
     * Set the text of the list items
     * @param text the text of the items
     * @see org.scilab.modules.gui.listbox.ListBox#setText(java.lang.String[])
     */
    public void setText(String[] text) {
        ScilabBridge.setText(this, text);
    }

    /**
     * Set the Relief of the ListBox
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        ScilabBridge.setRelief(this, reliefType);
    }

    /**
     * Delete the ListBox
     */
    public void destroy() {
        ScilabBridge.destroy(this);
    }

    /**
     * Give the focus to the ListBox
     */
    public void requestFocus() {
        ScilabBridge.requestFocus(this);
    }

    /**
     * Adjusts the view so that the element given by index is displayed at the top of the ListBox.
     * @param index the index of the element to be displayed at the top of the ListBox.
     */
    public void setListBoxTop(int index) {
        ScilabBridge.setListBoxTop(this, index);
    }

    /**
     * Gets the index of the element displayed at the top of the ListBox
     * @return the index of the element displayed at the top of the ListBox
     */
    public int getListBoxTop() {
        return ScilabBridge.getListBoxTop(this);
    }

    @Override
    public void setToolTipText(String tooltipText) {
        // TODO Auto-generated method stub
    }

    @Override
    public void resetBackground() {
        // TODO Auto-generated method stub

    }

    @Override
    public void resetForeground() {
        // TODO Auto-generated method stub

    }
}
