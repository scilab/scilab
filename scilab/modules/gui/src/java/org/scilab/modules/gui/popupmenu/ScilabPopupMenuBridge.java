/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.popupmenu;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab PopupMenus in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabPopupMenuBridge {

    /**
     * Constructor
     */
    protected ScilabPopupMenuBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab PopupMenu
     * @return the created PopupMenu
     */
    public static SimplePopupMenu createPopupMenu() {
        return new SwingScilabPopupMenu();
    }

    /**
     * Sets the Text of a Scilab PopupMenu
     * @param popupMenu the PopupMenu we want to set the Text of
     * @param newText the Text we want to set to the PopupMenu
     */
    public static void setText(PopupMenu popupMenu, String newText) {
        popupMenu.getAsSimplePopupMenu().setText(newText);
    }

    /**
     * Sets the Text of a Scilab PopupMenu
     * @param popupMenu the PopupMenu we want to get the Text of
     * @return the text of the PopupMenu
     */
    public static String getText(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getText();
    }

    /**
     * Draws a Scilab popupMenu
     * @param popupMenu the popupMenu to draw
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public static void draw(PopupMenu popupMenu) {
        popupMenu.getAsSimplePopupMenu().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab PopupMenu
     * @param popupMenu the popupMenu we want to get the dimensions of
     * @return the size of the popupMenu
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public static Size getDims(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab popupMenu
     * @param popupMenu the popupMenu we want to get the position of
     * @return the position of the popupMenu
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public static Position getPosition(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getPosition();
    }

    /**
     * Gets the visibility status of a Scilab PopupMenu
     * @param popupMenu the popupMenu we want to get the visiblity status of
     * @return the visibility status of the popupMenu (true if the popupMenu is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab PopupMenu
     * @param popupMenu the popupMenu we want to set the dimensions of
     * @param newSize the size we want to set to the popupMenu
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public static void setDims(PopupMenu popupMenu, Size newSize) {
        popupMenu.getAsSimplePopupMenu().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab popupMenu
     * @param popupMenu the popupMenu we want to set the position of
     * @param newPosition the position we want to set to the popupMenu
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public static void setPosition(PopupMenu popupMenu, Position newPosition) {
        popupMenu.getAsSimplePopupMenu().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab PopupMenu
     * @param popupMenu the popupMenu we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the popupMenu (true to set the popupMenu visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(PopupMenu popupMenu, boolean newVisibleState) {
        popupMenu.getAsSimplePopupMenu().setVisible(newVisibleState);
    }

    /**
     * Add a callback to the PopupMenu
     * @param popupMenu the PopupMenu we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(PopupMenu popupMenu, CommonCallBack callback) {
        popupMenu.getAsSimplePopupMenu().setCallback(callback);
    }

    /**
     * To set the Background color of the PopupMenu
     * @param popupMenu the PopupMenu we want to set the background of
     * @param color the Color
     */
    public static void setBackground(PopupMenu popupMenu, Color color) {
        popupMenu.getAsSimplePopupMenu().setBackground(color);
    }

    /**
     * Get the Background color of the PopupMenu
     * @param popupMenu the PopupMenu we want to get the background of
     * @return the Color
     */
    public static Color getBackground(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getBackground();
    }

    /**
     * To set the Foreground color of the popupMenu
     * @param popupMenu the PopupMenu we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(PopupMenu popupMenu, Color color) {
        popupMenu.getAsSimplePopupMenu().setForeground(color);
    }

    /**
     * Get the Foreground color of the PopupMenu
     * @param popupMenu the PopupMenu we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getForeground();
    }

    /**
     * Set the font of the PopupMenu.
     * @param popupMenu the PopupMenu we want to set the font of
     * @param font the font
     */
    public static void setFont(PopupMenu popupMenu, Font font) {
        popupMenu.getAsSimplePopupMenu().setFont(font);
    }

    /**
     * Get the font of the PopupMenu.
     * @param popupMenu the PopupMenu we want to get the font of
     * @return the font
     */
    public static Font getFont(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getFont();
    }

    /**
     * Set if the PopupMenu is enabled or not
     * @param popupMenu the PopupMenu we want to set the status of
     * @param status true if the PopupMenu is enabled
     */
    public static void setEnabled(PopupMenu popupMenu, boolean status) {
        popupMenu.getAsSimplePopupMenu().setEnabled(status);
    }

    /**
     * Gets the enable status of a PopupMenu
     * @param popupMenu the PopupMenu we want to get the status of
     * @return the enable status of the PopupMenu (true if the PopupMenu is enabled, false if not)
     */
    public static boolean isEnabled(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().isEnabled();
    }

    /**
     * Set the horizontal alignment for the PopupMenu text
     * @param popupMenu the PopupMenu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(PopupMenu popupMenu, String alignment) {
        popupMenu.getAsSimplePopupMenu().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the PopupMenu text
     * @param popupMenu the PopupMenu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(PopupMenu popupMenu, String alignment) {
        popupMenu.getAsSimplePopupMenu().setVerticalAlignment(alignment);
    }

    /**
     * Set the selected index of the ListBox
     * @param popupMenu the PopupMenu we want to set the selected index of
     * @param index the index of the item to be selected
     */
    public static void setUserSelectedIndex(PopupMenu popupMenu, int index) {
        popupMenu.getAsSimplePopupMenu().setUserSelectedIndex(index);
    }

    /**
     * Get the selected index of the ListBox
     * @param popupMenu the PopupMenu we want to get the selected index of
     * @return the index of the item selected
     */
    public static int getUserSelectedIndex(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getUserSelectedIndex();
    }

    /**
     * Get the text of all the PopupMenu items
     * @param popupMenu the PopupMenu we want to get the items of
     * @return the text items
     */
    public static String[] getAllItemsText(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getAllItemsText();
    }

    /**
     * Get the number of items in the PopupMenu
     * @param popupMenu the PopupMenu we want to get the items number of
     * @return the number of items
     */
    public static int getNumberOfItems(PopupMenu popupMenu) {
        return popupMenu.getAsSimplePopupMenu().getNumberOfItems();
    }

    /**
     * Set the text of the PopupMenu items
     * @param popupMenu the PopupMenu we want to set the text items of
     * @param text the text of the items
     */
    public static void setText(PopupMenu popupMenu, String[] text) {
        popupMenu.getAsSimplePopupMenu().setText(text);
    }

    /**
     * Set the Relief of the PopupMenu
     * @param popupMenu the PopupMenu which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(PopupMenu popupMenu, String reliefType) {
        popupMenu.getAsSimplePopupMenu().setRelief(reliefType);
    }

    /**
     * Delete the PopupMenu
     * @param popupMenu the PopupMenu to be destroyed
     */
    public static void destroy(PopupMenu popupMenu) {
        popupMenu.getAsSimplePopupMenu().destroy();
    }

    /**
     * Give the focus to the PopupMenu
     * @param popupMenu the PopupMenu
     */
    public static void requestFocus(PopupMenu popupMenu) {
        popupMenu.getAsSimplePopupMenu().requestFocus();
    }

}
