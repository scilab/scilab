/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.gui.checkboxmenuitem;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;

/**
 * Bridge for Scilab MenuItem in GUIs
 * @author Vincent COUVERT
 */
public class ScilabCheckBoxMenuItemBridge {

    /**
     * Constructor
     */
    protected ScilabCheckBoxMenuItemBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    };

    /**
     * Creates a Scilab MenuItem
     * @return the created MenuItem
     */
    public static SimpleCheckBoxMenuItem createCheckBoxMenuItem() {
        return createCheckBoxMenuItem(true);
    }

    /**
     * Creates a Scilab MenuItem
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     * @return the created MenuItem
     */
    public static SimpleCheckBoxMenuItem createCheckBoxMenuItem(boolean autoCheckedMode) {
        return new SwingScilabCheckBoxMenuItem(autoCheckedMode);
    }

    /**
     * Sets the Text of a Scilab MenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the Text of
     * @param newText the Text we want to set to the checkBoxMenuItem
     */
    public static void setText(CheckBoxMenuItem checkBoxMenuItem, String newText) {
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
                    checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setMnemonic(newText.charAt(charIndex + 1));

                    // Have to remove the & used to set a Mnemonic
                    String firstPart = newText.substring(0, Math.max(charIndex, 0)); // Before &
                    String secondPart = newText.substring(Math.min(charIndex + 1, newText.length()), newText.length()); // After &
                    label = firstPart + secondPart;
                    break;
                }

            }
        }

        // Set the text after relacing all && (display a & in the label) by &
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setText(label.replaceAll("&&", "&"));
    }

    /**
     * set a mnemonic to a CheckBoxMenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem which we want to add the mnemonic to
     * @param mnemonic the mnemonic to add to the CheckBoxMenuItem
     */
    public static void setMnemonic(CheckBoxMenuItem checkBoxMenuItem, char mnemonic) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setMnemonic(mnemonic);
    }

    /**
     * Get a text of a menu
     * @param checkBoxMenuItem the Menu which we want to set the text to
     * @return the text of the menu
     */
    public static String getText(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().getBaseText();
    }

    /**
     * set a mnemonic to a Menu
     * @param checkBoxMenuItem the Menu which we want to set the mnemonic to
     * @param mnemonic the mnemonic to set to the menu
     */
    public static void setMnemonic(CheckBoxMenuItem checkBoxMenuItem, int mnemonic) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setMnemonic(mnemonic);
    }

    /**
     * Set if the menu is enabled or not
     * @param checkBoxMenuItem the Menu which we want to add the mnemonic to
     * @param status true if the menu is enabled
     */
    public static void setEnabled(CheckBoxMenuItem checkBoxMenuItem, boolean status) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setEnabled(status);
    }

    /**
     * Get the enable status of a CheckBoxMenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem which we want to get the status of
     * @return the enable status of the CheckBoxMenuItem (true if the CheckBoxMenuItem is enabled, false if not)
     */
    public static boolean isEnabled(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().isEnabled();
    }

    /**
     * Gets the visibility status of a Scilab menu
     * @param checkBoxMenuItem the menu we want to get the visiblity status of
     * @return the visibility status of the menu (true if the menu is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().isVisible();
    }

    /**
     * Sets the visibility status of a Scilab menu
     * @param checkBoxMenuItem the menu we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the menu (true to set the menu visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(CheckBoxMenuItem checkBoxMenuItem, boolean newVisibleState) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setVisible(newVisibleState);
    }

    /**
     * Set the Background color of the menu
     * @param checkBoxMenuItem the menu we want to set the background of
     * @param color the Color
     */
    public static void setBackground(CheckBoxMenuItem checkBoxMenuItem, Color color) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setBackground(color);
    }

    /**
     * Get the Background color of the menu
     * @param checkBoxMenuItem the menu we want to get the background of
     * @return the Color
     */
    public static Color getBackground(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().getBackground();
    }

    /**
     * Set the Foreground color of the menu
     * @param checkBoxMenuItem the menu we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(CheckBoxMenuItem checkBoxMenuItem, Color color) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setForeground(color);
    }

    /**
     * Get the Foreground color of the menu
     * @param checkBoxMenuItem the menu we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().getForeground();
    }

    /**
     * Set the font of the checkBoxMenuItem.
     * @param checkBoxMenuItem the menu we want to set the font of
     * @param font the font
     */
    public static void setFont(CheckBoxMenuItem checkBoxMenuItem, Font font) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setFont(font);
    }

    /**
     * Get the font of the checkBoxMenuItem.
     * @param checkBoxMenuItem the menu we want to get the font of
     * @return the font
     */
    public static Font getFont(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().getFont();
    }

    /**
     * Add a callback to the menu, this callback is a Scilab command
     * @param checkBoxMenuItem the menu we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(CheckBoxMenuItem checkBoxMenuItem, CommonCallBack callback) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setCallback(callback);
    }

    /**
     * Set the horizontal alignment for the Menu text
     * @param checkBoxMenuItem the Menu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(CheckBoxMenuItem checkBoxMenuItem, String alignment) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the Menu text
     * @param checkBoxMenuItem the Menu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(CheckBoxMenuItem checkBoxMenuItem, String alignment) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the CheckBoxMenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(CheckBoxMenuItem checkBoxMenuItem, String reliefType) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setRelief(reliefType);
    }

    /**
     * Add a CheckBoxMenuItem to the CheckBoxMenuItem
     * @param parentCheckBoxMenuItem the CheckBoxMenuItem where we want to add a CheckBoxMenuItem
     * @param childCheckBoxMenuItem the CheckBoxMenuItem we want to add
     */
    public static void add(CheckBoxMenuItem parentCheckBoxMenuItem, CheckBoxMenuItem childCheckBoxMenuItem) {
        parentCheckBoxMenuItem.getAsSimpleCheckBoxMenuItem().add(childCheckBoxMenuItem);
    }

    /**
     * Add a Menu to the CheckBoxMenuItem
     * @param parentCheckBoxMenuItem the CheckBoxMenuItem where we want to add a CheckBoxMenuItem
     * @param childMenu the Menu we want to add
     */
    public static void add(CheckBoxMenuItem parentCheckBoxMenuItem, Menu childMenu) {
        parentCheckBoxMenuItem.getAsSimpleCheckBoxMenuItem().add(childMenu);
    }

    /**
     * Destroy the CheckBoxMenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem we want to destroy
     */
    public static void destroy(CheckBoxMenuItem checkBoxMenuItem) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().destroy();
    }

    /**
     * Give the focus to the CheckBoxMenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem
     */
    public static void requestFocus(CheckBoxMenuItem checkBoxMenuItem) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().requestFocus();
    }

    /**
     * Retrieve the CallBack associated to this CheckBoxMenuItem
     * @param checkBoxMenuItem the CheckBoxMenuItem
     * @return the CallBack
     */
    public static CommonCallBack getCallback(CheckBoxMenuItem checkBoxMenuItem) {
        return checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().getCallback();
    }

    /**
     * Set if the menu item is checked or not
     * @param checkBoxMenuItem the CheckBoxMenuItem
     * @param status true if the menu item is checked
     */
    public static void setChecked(CheckBoxMenuItem checkBoxMenuItem, boolean status) {
        checkBoxMenuItem.getAsSimpleCheckBoxMenuItem().setChecked(status);
    }

    /**
     * Get if the menu item is checked or not
     * @return true if the menu item is checked
     */
    public static boolean isChecked(CheckBoxMenuItem checkBoxCheckBoxMenuItem) {
        return checkBoxCheckBoxMenuItem.getAsSimpleCheckBoxMenuItem().isChecked();
    }

}
