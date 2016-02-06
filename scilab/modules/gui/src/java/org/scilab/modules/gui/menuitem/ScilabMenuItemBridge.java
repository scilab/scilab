/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.menuitem;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;

/**
 * Bridge for Scilab MenuItem in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuItemBridge {

    /**
     * Constructor
     */
    protected ScilabMenuItemBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    };

    /**
     * Creates a Scilab MenuItem
     * @return the created MenuItem
     */
    public static SimpleMenuItem createMenuItem() {
        return createMenuItem(true);
    }

    /**
     * Creates a Scilab MenuItem
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     * @return the created MenuItem
     */
    public static SimpleMenuItem createMenuItem(boolean autoCheckedMode) {
        return new SwingScilabMenuItem(autoCheckedMode);
    }

    /**
     * Sets the Text of a Scilab MenuItem
     * @param menuItem the MenuItem we want to set the Text of
     * @param newText the Text we want to set to the menuItem
     */
    public static void setText(MenuItem menuItem, String newText) {
        String label = newText;

        // Try to set a mnemonic according to text (character preeceded by a &)
        for (int charIndex = 0; charIndex < newText.length(); charIndex++) {
            if (newText.charAt(charIndex) == '&') {

                boolean canBeAMnemonic = true;

                // Previous char must not be a &
                if ((charIndex != 0) && (newText.charAt(charIndex - 1) == '&')) {
                    canBeAMnemonic = false;
                }

                if (canBeAMnemonic && newText.charAt(charIndex + 1) != '&') {
                    // A mnemonic
                    menuItem.getAsSimpleMenuItem().setMnemonic(newText.charAt(charIndex + 1));

                    // Have to remove the & used to set a Mnemonic
                    String firstPart = newText.substring(0, Math.max(charIndex, 0)); // Before &
                    String secondPart = newText.substring(Math.min(charIndex + 1, newText.length()), newText.length()); // After &
                    label = firstPart + secondPart;
                    break;
                }

            }
        }

        // Set the text after relacing all && (display a & in the label) by &
        menuItem.getAsSimpleMenuItem().setText(label.replaceAll("&&", "&"));
    }

    /**
     * set a mnemonic to a MenuItem
     * @param menuItem the MenuItem which we want to add the mnemonic to
     * @param mnemonic the mnemonic to add to the MenuItem
     */
    public static void setMnemonic(MenuItem menuItem, char mnemonic) {
        menuItem.getAsSimpleMenuItem().setMnemonic(mnemonic);
    }

    /**
     * Get a text of a menu
     * @param menuItem the Menu which we want to set the text to
     * @return the text of the menu
     */
    public static String getText(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().getBaseText();
    }

    /**
     * set a mnemonic to a Menu
     * @param menuItem the Menu which we want to set the mnemonic to
     * @param mnemonic the mnemonic to set to the menu
     */
    public static void setMnemonic(MenuItem menuItem, int mnemonic) {
        menuItem.getAsSimpleMenuItem().setMnemonic(mnemonic);
    }

    /**
     * Set if the menu is enabled or not
     * @param menuItem the Menu which we want to add the mnemonic to
     * @param status true if the menu is enabled
     */
    public static void setEnabled(MenuItem menuItem, boolean status) {
        menuItem.getAsSimpleMenuItem().setEnabled(status);
    }

    /**
     * Get the enable status of a MenuItem
     * @param menuItem the MenuItem which we want to get the status of
     * @return the enable status of the MenuItem (true if the MenuItem is enabled, false if not)
     */
    public static boolean isEnabled(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().isEnabled();
    }

    /**
     * Gets the visibility status of a Scilab menu
     * @param menuItem the menu we want to get the visiblity status of
     * @return the visibility status of the menu (true if the menu is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().isVisible();
    }

    /**
     * Sets the visibility status of a Scilab menu
     * @param menuItem the menu we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the menu (true to set the menu visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(MenuItem menuItem, boolean newVisibleState) {
        menuItem.getAsSimpleMenuItem().setVisible(newVisibleState);
    }

    /**
     * Set the Background color of the menu
     * @param menuItem the menu we want to set the background of
     * @param color the Color
     */
    public static void setBackground(MenuItem menuItem, Color color) {
        menuItem.getAsSimpleMenuItem().setBackground(color);
    }

    /**
     * Get the Background color of the menu
     * @param menuItem the menu we want to get the background of
     * @return the Color
     */
    public static Color getBackground(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().getBackground();
    }

    /**
     * Set the Foreground color of the menu
     * @param menuItem the menu we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(MenuItem menuItem, Color color) {
        menuItem.getAsSimpleMenuItem().setForeground(color);
    }

    /**
     * Get the Foreground color of the menu
     * @param menuItem the menu we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().getForeground();
    }

    /**
     * Set the font of the menuItem.
     * @param menuItem the menu we want to set the font of
     * @param font the font
     */
    public static void setFont(MenuItem menuItem, Font font) {
        menuItem.getAsSimpleMenuItem().setFont(font);
    }

    /**
     * Get the font of the menuItem.
     * @param menuItem the menu we want to get the font of
     * @return the font
     */
    public static Font getFont(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().getFont();
    }

    /**
     * Add a callback to the menu, this callback is a Scilab command
     * @param menuItem the menu we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(MenuItem menuItem, CommonCallBack callback) {
        menuItem.getAsSimpleMenuItem().setCallback(callback);
    }

    /**
     * Set the horizontal alignment for the Menu text
     * @param menuItem the Menu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(MenuItem menuItem, String alignment) {
        menuItem.getAsSimpleMenuItem().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the Menu text
     * @param menuItem the Menu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(MenuItem menuItem, String alignment) {
        menuItem.getAsSimpleMenuItem().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the MenuItem
     * @param menuItem the MenuItem which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(MenuItem menuItem, String reliefType) {
        menuItem.getAsSimpleMenuItem().setRelief(reliefType);
    }

    /**
     * Add a MenuItem to the MenuItem
     * @param parentMenuItem the MenuItem where we want to add a MenuItem
     * @param childMenuItem the MenuItem we want to add
     */
    public static void add(MenuItem parentMenuItem, MenuItem childMenuItem) {
        parentMenuItem.getAsSimpleMenuItem().add(childMenuItem);
    }

    /**
     * Add a Menu to the MenuItem
     * @param parentMenuItem the MenuItem where we want to add a MenuItem
     * @param childMenu the Menu we want to add
     */
    public static void add(MenuItem parentMenuItem, Menu childMenu) {
        parentMenuItem.getAsSimpleMenuItem().add(childMenu);
    }

    /**
     * Destroy the MenuItem
     * @param menuItem the MenuItem we want to destroy
     */
    public static void destroy(MenuItem menuItem) {
        menuItem.getAsSimpleMenuItem().destroy();
    }

    /**
     * Give the focus to the MenuItem
     * @param menuItem the MenuItem
     */
    public static void requestFocus(MenuItem menuItem) {
        menuItem.getAsSimpleMenuItem().requestFocus();
    }

    /**
     * Retrieve the CallBack associated to this MenuItem
     * @param menuItem the MenuItem
     * @return the CallBack
     */
    public static CommonCallBack getCallback(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().getCallback();
    }

    /**
     * Set if the MenuItem is checked or not
     * @param menu the MenuItem
     * @param status true if the MenuItem is checked
     */
    public static void setChecked(MenuItem menuItem, boolean status) {
        menuItem.getAsSimpleMenuItem().setChecked(status);
    }

    /**
     * Get if the MenuItem is checked or not
     * @return true if the MenuItem is checked
     */
    public static boolean isChecked(MenuItem menuItem) {
        return menuItem.getAsSimpleMenuItem().isChecked();
    }

}
