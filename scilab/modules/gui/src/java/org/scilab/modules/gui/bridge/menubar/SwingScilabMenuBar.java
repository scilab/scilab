/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.gui.bridge.menubar;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;

import java.awt.Container;

import javax.swing.JMenu;
import javax.swing.JMenuBar;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuBar extends JMenuBar implements SimpleMenuBar {

    private static final long serialVersionUID = 1L;

    private int elementId;

    /**
     * Constructor
     */
    public SwingScilabMenuBar() {
        super();
    }

    /**
     * Append a Menu to a MenuBar
     * @param newMenu the Menu to add to the MenuBar
     * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.ContextMenu)
     */
    public void add(Menu newMenu) {
        this.add((SwingScilabMenu) newMenu.getAsSimpleMenu());
    }

    /**
     * Append a Menu to a MenuBar
     * @param newMenu the Menu to add to the MenuBar
     * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.ContextMenu)
     */
    public void add(SwingScilabMenu newMenu) {
        //ScilabSwingUtilities.addToParent(newMenu, this);
        ((Container) this).add(newMenu);
    }

    /**
     * Enabale/Disable a menu giving its name
     * @param menuName the name of the menu
     * @param status true to enable the menu
     */
    public void setMenuEnabled(String menuName, boolean status) {

        for (int menuIndex = 0; menuIndex < this.getMenuCount(); menuIndex++) {
            // Check the name of each menu until one matches the name
            if (this.getMenu(menuIndex).getText().equals(removeMnemonicFromName(menuName))) {
                Integer id = ((SwingScilabMenu) this.getMenu(menuIndex)).getId();
                GraphicController.getController().setProperty(id, __GO_UI_ENABLE__, status);
                break;
            }
        }

    }

    /**
     * Disable a MenuItem of a Scilab root window giving its parent name and position
     * @param parentMenuName the name of the parent menu
     * @param menuItemPosition the name of the parent menu
     * @param status true to set the menu enabled
     */
    public void setSubMenuEnabled(String parentMenuName, int menuItemPosition, boolean status) {
        JMenu parentMenu = null;

        for (int menuIndex = 0; menuIndex < this.getMenuCount(); menuIndex++) {
            // Check the name of each menu until one matches the name
            if (this.getMenu(menuIndex).getText().equals(removeMnemonicFromName(parentMenuName))) {
                parentMenu = this.getMenu(menuIndex);
                break;
            }
        }
        if (parentMenu == null) { /* Parent does not exist */
            return;
        }
        if ((menuItemPosition <= parentMenu.getItemCount()) & (menuItemPosition >= 1)) {
            /* Java index begins at 0 and Scilab index begins at 1 */
            if (parentMenu.getItem(menuItemPosition - 1) != null) {
                Integer id = ((SwingScilabMenuItem) parentMenu.getItem(menuItemPosition - 1)).getId();
                GraphicController.getController().setProperty(id, __GO_UI_ENABLE__, status);
            }
        }
    }


    /**
     * Remove a menu giving its name
     * @param menuName the name of the menu
     */
    public void removeMenu(String menuName) {
        for (int menuIndex = 0; menuIndex < this.getMenuCount(); menuIndex++) {
            // Check the name of each menu until one matches the name
            if (this.getMenu(menuIndex).getText().equals(removeMnemonicFromName(menuName))) {
                this.getMenu(menuIndex).setVisible(false);
                Integer id = ((SwingScilabMenu) this.getMenu(menuIndex)).getId();
                GraphicController.getController().removeRelationShipAndDelete(id);
                break;
            }
        }

    }

    /**
     * Remove Mnemonic & from name
     * @param menuName the name of the menu
     * @return the name without & used to set Mnemonics
     */
    private String removeMnemonicFromName(String menuName) {
        String label = menuName;

        // Try to set a mnemonic according to text (character preeceded by a &)
        for (int charIndex = 0; charIndex < menuName.length(); charIndex++) {
            if (menuName.charAt(charIndex) == '&') {

                boolean canBeAMnemonic = true;

                // Previous char must not be a &
                if ((charIndex != 0) && (menuName.charAt(charIndex - 1) == '&')) {
                    canBeAMnemonic = false;
                }

                if ((charIndex + 1 < menuName.length()) && (canBeAMnemonic && menuName.charAt(charIndex + 1) != '&')) {
                    // A mnemonic
                    // Have to remove the & used to set a Mnemonic
                    String firstPart = menuName.substring(0, Math.max(charIndex, 0)); // Before &
                    String secondPart = menuName.substring(Math.min(charIndex + 1, menuName.length()), menuName.length());
                    label = firstPart + secondPart;
                    break;
                }

            }
        }

        return label.replaceAll("&&", "&");
    }

    public void close() {
        UIElementMapper.removeMapping(elementId);
    }
}
