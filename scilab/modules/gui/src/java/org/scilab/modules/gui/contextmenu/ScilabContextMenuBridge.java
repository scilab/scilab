/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.contextmenu;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Bridge for Scilab Menus in GUIs
 * @author Vincent COUVERT
 */
public class ScilabContextMenuBridge {

    /**
     * Constructor
     */
    protected ScilabContextMenuBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab ContextMenu
     * @return the created ContextMenu
     */
    public static SimpleContextMenu createContextMenu() {
        return new SwingScilabContextMenu();
    }

    /**
     * set a text to a ContextMenu
     * @param contextMenu the ContextMenu which we want to set the text to
     * @param newText the new text to set to the contextMenu
     */
    public static void setText(ContextMenu contextMenu, String newText) {
        String label = newText;
        // TODO mnemonic management
        // Set the text after relacing all && (display a & in the label) by &
        contextMenu.getAsSimpleContextMenu().setText(label.replaceAll("&&", "&"));
    }

    /**
     * Get a text of a ContextMenu
     * @param contextMenu the ContextMenu which we want to set the text to
     * @return the text of the contextMenu
     */
    public static String getText(ContextMenu contextMenu) {
        return contextMenu.getAsSimpleContextMenu().getText();
    }

    /**
     * Append a MenuItem to a Scilab ContextMenu
     * @param contextMenu the ContextMenu which we want to add the ContextMenuItem to
     * @param newMenuItem the MenuItem to add to the ContextMenu
     */
    public static void add(ContextMenu contextMenu, MenuItem newMenuItem) {
        contextMenu.getAsSimpleContextMenu().add(newMenuItem);
    }

    /**
     * Append a Menu to a Scilab ContextMenu
     * @param contextMenu the ContextMenu which we want to add the ContextMenuItem to
     * @param newMenu the Menu to add to the ContextMenu
     */
    public static void add(ContextMenu contextMenu, Menu newMenu) {
        contextMenu.getAsSimpleContextMenu().add(newMenu);
    }

    /**
     * Set if the ContextMenu is enabled or not
     * @param contextMenu the ContextMenu which we want to set the status of
     * @param status true if the contextMenu is enabled
     */
    public static void setEnabled(ContextMenu contextMenu, boolean status) {
        contextMenu.getAsSimpleContextMenu().setEnabled(status);
    }

    /**
     * Gets the enable status of a ContextMenu
     * @param contextMenu the contextMenu which we want to get the status of
     * @return the enable status of the ContextMenu (true if the ContextMenu is enabled, false if not)
     */
    public static boolean isEnabled(ContextMenu contextMenu) {
        return contextMenu.getAsSimpleContextMenu().isEnabled();
    }

    /**
     * Gets the visibility status of a Scilab ContextMenu
     * @param contextMenu the contextMenu we want to get the visiblity status of
     * @return the visibility status of the contextMenu (true if the contextMenu is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(ContextMenu contextMenu) {
        return contextMenu.getAsSimpleContextMenu().isVisible();
    }

    /**
     * Sets the visibility status of a Scilab ContextMenu
     * @param contextMenu the contextMenu we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the contextMenu (true to set the contextMenu visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(ContextMenu contextMenu, boolean newVisibleState) {
        contextMenu.getAsSimpleContextMenu().setVisible(newVisibleState);
    }

    /**
     * Set the Background color of the contextMenu
     * @param contextMenu the contextMenu we want to set the background of
     * @param color the Color
     */
    public static void setBackground(ContextMenu contextMenu, Color color) {
        contextMenu.getAsSimpleContextMenu().setBackground(color);
    }

    /**
     * Get the Background color of the contextMenu
     * @param contextMenu the contextMenu we want to get the background of
     * @return the Color
     */
    public static Color getBackground(ContextMenu contextMenu) {
        return contextMenu.getAsSimpleContextMenu().getBackground();
    }

    /**
     * Set the Foreground color of the contextMenu
     * @param contextMenu the contextMenu we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(ContextMenu contextMenu, Color color) {
        contextMenu.getAsSimpleContextMenu().setForeground(color);
    }

    /**
     * Get the Foreground color of the contextMenu
     * @param contextMenu the contextMenu we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(ContextMenu contextMenu) {
        return contextMenu.getAsSimpleContextMenu().getForeground();
    }

    /**
     * Set the font of the contextMenu.
     * @param contextMenu the contextMenu we want to set the font of
     * @param font the font
     */
    public static void setFont(ContextMenu contextMenu, Font font) {
        contextMenu.getAsSimpleContextMenu().setFont(font);
    }

    /**
     * Get the font of the contextMenu.
     * @param contextMenu the contextMenu we want to get the font of
     * @return the font
     */
    public static Font getFont(ContextMenu contextMenu) {
        return contextMenu.getAsSimpleContextMenu().getFont();
    }

    /**
     * Add a callback to the contextMenu, this callback is a Scilab command
     * @param contextMenu the contextMenu we want to set the callback of
     * @param callback the Callback to set.
     */
    public static void setCallback(ContextMenu contextMenu, CommonCallBack callback) {
        contextMenu.getAsSimpleContextMenu().setCallback(callback);
    }

    /**
     * Set the horizontal alignment for the ContextMenu text
     * @param contextMenu the ContextMenu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(ContextMenu contextMenu, String alignment) {
        contextMenu.getAsSimpleContextMenu().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the ContextMenu text
     * @param contextMenu the ContextMenu we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(ContextMenu contextMenu, String alignment) {
        contextMenu.getAsSimpleContextMenu().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the ContextMenu
     * @param contextMenu the ContextMenu which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(ContextMenu contextMenu, String reliefType) {
        contextMenu.getAsSimpleContextMenu().setRelief(reliefType);
    }

    /**
     * Delete the ContextMenu
     * @param contextMenu the ContextMenu to be destroyed
     */
    public static void destroy(ContextMenu contextMenu) {
        contextMenu.getAsSimpleContextMenu().destroy();
    }

    /**
     * Give the focus to the ContextMenu
     * @param contextMenu the ContextMenu
     */
    public static void requestFocus(ContextMenu contextMenu) {
        contextMenu.getAsSimpleContextMenu().requestFocus();
    }

}
