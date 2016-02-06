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

package org.scilab.modules.gui.menu;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Bridge for Scilab Menus in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBridge {

    /**
     * Constructor
     */
    protected ScilabMenuBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab Menu
     * @return the created Menu
     */
    public static SimpleMenu createMenu() {
        return new SwingScilabMenu();
    }

    /**
     * set a text to a menu
     * @param menu the Menu which we want to set the text to
     * @param newText the new text to set to the menu
     */
    public static void setText(Menu menu, String newText) {
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
                    menu.getAsSimpleMenu().setMnemonic(newText.charAt(charIndex + 1));

                    // Have to remove the & used to set a Mnemonic
                    String firstPart = newText.substring(0, Math.max(charIndex, 0)); // Before &
                    String secondPart = newText.substring(Math.min(charIndex + 1, newText.length()), newText.length()); // After &
                    label = firstPart + secondPart;
                    break;
                }

            }
        }

        // Set the text after relacing all && (display a & in the label) by &
        menu.getAsSimpleMenu().setText(label.replaceAll("&&", "&"));
    }

    /**
     * Get a text of a menu
     * @param menu the Menu which we want to set the text to
     * @return the text of the menu
     */
    public static String getText(Menu menu) {
        return menu.getAsSimpleMenu().getBaseText();
    }

    /**
     * Append a MenuItem to a Scilab Menu
     * @param menu the Menu which we want to add the MenuItem to
     * @param newMenuItem the MenuItem to add to the Menu
     */
    public static void add(Menu menu, MenuItem newMenuItem) {
        menu.getAsSimpleMenu().add(newMenuItem);
    }

    /**
     * Append a CheckBoxMenuItem to a Scilab Menu
     * @param menu the Menu which we want to add the CheckBoxMenuItem to
     * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the Menu
     */
    public static void add(Menu menu, CheckBoxMenuItem newCheckBoxMenuItem) {
        menu.getAsSimpleMenu().add(newCheckBoxMenuItem);
    }

    /**
     * Append a subMenu to a Scilab Menu
     * @param menu the Menu which we want to append the subMenu to
     * @param newSubMenu the subMenu to append to the Menu
     */
    public static void add(Menu menu, Menu newSubMenu) {
        menu.getAsSimpleMenu().add(newSubMenu);
    }

    /**
     * set a mnemonic to a Menu
     * @param menu the Menu which we want to set the mnemonic to
     * @param mnemonic the mnemonic to set to the menu
     */
    public static void setMnemonic(Menu menu, int mnemonic) {
        menu.getAsSimpleMenu().setMnemonic(mnemonic);
    }

    /**
     * Add a Separator to a Menu
     * @param menu the Menu which we want to add the Separator to
     */
    public static void addSeparator(Menu menu) {
        menu.getAsSimpleMenu().addSeparator();
    }

    /**
     * Set if the menu is enabled or not
     * @param menu the Menu which we want to set the status of
     * @param status true if the menu is enabled
     */
    public static void setEnabled(Menu menu, boolean status) {
        menu.getAsSimpleMenu().setEnabled(status);
    }

    /**
     * Gets the enable status of a Menu
     * @param menu the menu which we want to get the status of
     * @return the enable status of the Menu (true if the Menu is enabled, false if not)
     */
    public static boolean isEnabled(Menu menu) {
        return menu.getAsSimpleMenu().isEnabled();
    }

    /**
     * Gets the visibility status of a Scilab menu
     * @param menu the menu we want to get the visiblity status of
     * @return the visibility status of the menu (true if the menu is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(Menu menu) {
        return menu.getAsSimpleMenu().isVisible();
    }

    /**
     * Sets the visibility status of a Scilab menu
     * @param menu the menu we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the menu (true to set the menu visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(Menu menu, boolean newVisibleState) {
        menu.getAsSimpleMenu().setVisible(newVisibleState);
    }

    /**
     * Set the Background color of the menu
     * @param menu the menu we want to set the background of
     * @param color the Color
     */
    public static void setBackground(Menu menu, Color color) {
        menu.getAsSimpleMenu().setBackground(color);
    }

    /**
     * Get the Background color of the menu
     * @param menu the menu we want to get the background of
     * @return the Color
     */
    public static Color getBackground(Menu menu) {
        return menu.getAsSimpleMenu().getBackground();
    }

    /**
     * Set the Foreground color of the menu
     * @param menu the menu we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(Menu menu, Color color) {
        menu.getAsSimpleMenu().setForeground(color);
    }

    /**
     * Get the Foreground color of the menu
     * @param menu the menu we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(Menu menu) {
        return menu.getAsSimpleMenu().getForeground();
    }

    /**
     * Set the font of the menu.
     * @param menu the menu we want to set the font of
     * @param font the font
     */
    public static void setFont(Menu menu, Font font) {
        menu.getAsSimpleMenu().setFont(font);
    }

    /**
     * Get the font of the menu.
     * @param menu the menu we want to get the font of
     * @return the font
     */
    public static Font getFont(Menu menu) {
        return menu.getAsSimpleMenu().getFont();
    }

    /**
     * Add a callback to the menu, this callback is a Scilab command
     * @param menu the menu we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(Menu menu, CommonCallBack callback) {
        menu.getAsSimpleMenu().setCallback(callback);
    }

    /**
     * Set the horizontal alignment for the Menu text
     * @param menu the Menu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(Menu menu, String alignment) {
        menu.getAsSimpleMenu().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the Menu text
     * @param menu the Menu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(Menu menu, String alignment) {
        menu.getAsSimpleMenu().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the Menu
     * @param menu the Menu which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(Menu menu, String reliefType) {
        menu.getAsSimpleMenu().setRelief(reliefType);
    }

    /**
     * Delete the Menu
     * @param menu the Menu to be destroyed
     */
    public static void destroy(Menu menu) {
        menu.getAsSimpleMenu().destroy();
    }

    /**
     * Give the focus to the Menu
     * @param menu the Menu
     */
    public static void requestFocus(Menu menu) {
        menu.getAsSimpleMenu().requestFocus();
    }

    /**
     * Retrieve the CallBack associated to this Menu
     * @param menuItem the Menu
     * @return the CallBack
     */
    public static CommonCallBack getCallback(Menu menu) {
        return menu.getAsSimpleMenu().getCallback();
    }

    /**
     * Set if the Menu is checked or not
     * @param menu the Menu
     * @param status true if the Menu is checked
     */
    public static void setChecked(Menu menu, boolean status) {
        menu.getAsSimpleMenu().setChecked(status);
    }

    /**
     * Get if the Menu is checked or not
     * @return true if the Menu is checked
     */
    public static boolean isChecked(Menu menu) {
        return menu.getAsSimpleMenu().isChecked();
    }

}
