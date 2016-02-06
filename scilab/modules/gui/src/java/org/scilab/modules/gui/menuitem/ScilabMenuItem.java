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

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab MenuItem in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuItem extends ScilabMenu implements MenuItem {

    private SimpleMenuItem component;

    /**
     * Constructor
     */
    protected ScilabMenuItem() {
        this(true);
    }

    /**
     * Constructor
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     */
    protected ScilabMenuItem(boolean autoCheckedMode) {
        component = ScilabBridge.createMenuItem(autoCheckedMode);
    }
    /**
     * Creates a Scilab MenuItem object
     * @return the created MenuItem
     */
    public static MenuItem createMenuItem() {
        return createMenuItem(true);
    }

    /**
     * Creates a Scilab MenuItem object
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     * @return the created MenuItem
     */
    public static MenuItem createMenuItem(boolean autoCheckedMode) {
        return new ScilabMenuItem(autoCheckedMode);
    }
    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleMenuItem getAsSimpleMenuItem() {
        return component;
    }

    /**
     * Sets the text of a Scilab Menu
     * @param newText the Text to set to the Menu
     * @see org.scilab.modules.gui.menu.Menu#setText(java.lang.String)
     */
    public void setText(String newText) {
        ScilabBridge.setText(this, newText);
    }

    /**
     * Gets the text of a Scilab Menu
     * @return the Text to set to the Menu
     * @see org.scilab.modules.gui.menu.Menu#getText(java.lang.String)
     */
    public String getText() {
        return ScilabBridge.getText(this);
    }

    /**
     * set a mnemonic to a Menu
     * @param mnemonic the mnemonic to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
     */
    public void setMnemonic(char mnemonic) {
        ScilabBridge.setMnemonic(this, mnemonic);
    }

    /**
     * set a mnemonic to a Menu
     * @param mnemonic the mnemonic to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
     */
    public void setMnemonic(int mnemonic) {
        ScilabBridge.setMnemonic(this, mnemonic);
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public boolean isVisible() {
        return ScilabBridge.isVisible(this);
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newSize is not used
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newPosition is not used
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newVisibleState is not used
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }

    /**
     * Set the Background color of the menu
     * @param color the Color
     */
    public void setBackground(Color color) {
        ScilabBridge.setBackground(this, color);
    }

    /**
     * Get the Background color of the menu
     * @return the Color
     */
    public Color getBackground() {
        return ScilabBridge.getBackground(this);
    }

    /**
     * Set the Foreground color of the menu
     * @param color the Color
     */
    public void setForeground(Color color) {
        ScilabBridge.setForeground(this, color);
    }

    /**
     * Get the Foreground color of the menu
     * @return the Color
     */
    public Color getForeground() {
        return ScilabBridge.getForeground(this);
    }

    /**
     * Set the font of the menu.
     * @param font the font
     */
    public void setFont(Font font) {
        ScilabBridge.setFont(this, font);
    }

    /**
     * Get the font of the menu.
     * @return the font
     */
    public Font getFont() {
        return ScilabBridge.getFont(this);
    }

    /**
     * Add a callback to the menu, this callback is a Scilab command
     * @param callback the CallBack to set.
     */
    public void setCallback(CommonCallBack callback) {
        ScilabBridge.setCallback(this, callback);
    }

    /**
     * Set the horizontal alignment for the Menu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        ScilabBridge.setHorizontalAlignment(this, alignment);
    }

    /**
     * Set the vertical alignment for the Menu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        ScilabBridge.setVerticalAlignment(this, alignment);
    }

    /**
     * Set the Relief of the Menu
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        ScilabBridge.setRelief(this, reliefType);
    }

    /**
     * Set if the menu item is enabled or not
     * @param status true if the menu item is enabled
     */
    public void setEnabled(boolean status) {
        ScilabBridge.setEnabled(this, status);
    }

    /**
     * Gets the enable status of a MenuItem
     * @return the enable status of the MenuItem (true if the MenuItem is enabled, false if not)
     */
    public boolean isEnabled() {
        return ScilabBridge.isEnabled(this);
    }

    /**
     * Add a Menu to this MenuItem
     * @param childMenu the Menu we want to add
     */
    public void add(Menu childMenu) {
        ScilabBridge.add(this, childMenu);
    }

    /**
     * Add a MenuItem to this MenuItem
     * @param childMenuItem the MenuItem we want to add
     */
    public void add(MenuItem childMenuItem) {
        ScilabBridge.add(this, childMenuItem);
    }

    /**
     * Destroy the MenuItem
     */
    public void destroy() {
        ScilabBridge.destroy(this);
    }

    /**
     * Give the focus to the ContextMenu
     */
    public void requestFocus() {
        ScilabBridge.requestFocus(this);
    }

    /**
     * Retrieve the CallBack associated to this MenuItem
     * @return the CallBack
     */
    public CommonCallBack getCallback() {
        return ScilabBridge.getCallback(this);
    }

    /**
     * Set if the Menu is checked or not
     * @param status true if the Menu is checked
     */
    public void setChecked(boolean status) {
        ScilabBridge.setChecked(this, status);
    }

    /**
     * Get if the Menu is checked or not
     * @return true if the Menu is checked
     */
    public boolean isChecked() {
        return ScilabBridge.isChecked(this);
    }
}
