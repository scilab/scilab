/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2008 - INRIA - Marouane BEN JELLOUL
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

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;

/**
 * Class for Scilab Menus in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabContextMenu extends ScilabWidget implements ContextMenu {

    private SimpleContextMenu component;

    /**
     * Constructor
     */
    protected ScilabContextMenu() {
        component = ScilabBridge.createContextMenu();
    }

    /**
     * Creates a Scilab ContextMenu object
     * @return the created ContextMenu
     */
    public static ContextMenu createContextMenu() {
        return new ScilabContextMenu();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleContextMenu getAsSimpleContextMenu() {
        return component;
    }

    /**
     * Append a MenuItem to a Scilab ContextMenu
     * @param newMenuItem the MenuItem to add to the ContextMenu
     */
    public void add(MenuItem newMenuItem) {
        ScilabBridge.add(this, newMenuItem);
    }

    /**
     * Append a Menu to the ContextMenu
     * @param newMenu the Menu to add to the ContextMenu
     */
    public void add(Menu newMenu) {
        ScilabBridge.add(this, newMenu);
    }

    /**
     * Sets the text of a Scilab ContextMenu
     * @param newText the Text to set to the ContextMenu
     */
    public void setText(String newText) {
        ScilabBridge.setText(this, newText);
    }

    public void setEmptyText() {
        setText(null);
    }

    /**
     * Gets the text of a Scilab ContextMenu
     * @return the Text to set to the ContextMenu
     */
    public String getText() {
        return ScilabBridge.getText(this);
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
     * Get the visibility status of the ContextMenu
     * @return true if the ContextMenu is visible
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
     * Set the visibility status of the ContextMenu
     * @param newVisibleState is true to set the ContextMenu visible
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }

    /**
     * Enable/Disable the ContextMenu
     * @param status true to set the ContextMenu enabled
     */
    public void setEnabled(boolean status) {
        ScilabBridge.setEnabled(this, status);
    }

    /**
     * Gets the enable status of a ContextMenu
     * @return the enable status of the ContextMenu (true if the ContextMenu is enabled, false if not)
     */
    public boolean isEnabled() {
        return ScilabBridge.isEnabled(this);
    }

    /**
     * Set the Background color of the ContextMenu
     * @param color the Color
     */
    public void setBackground(Color color) {
        ScilabBridge.setBackground(this, color);
    }

    /**
     * Get the Background color of the ContextMenu
     * @return the Color
     */
    public Color getBackground() {
        return ScilabBridge.getBackground(this);
    }

    /**
     * Set the Foreground color of the ContextMenu
     * @param color the Color
     */
    public void setForeground(Color color) {
        ScilabBridge.setForeground(this, color);
    }

    /**
     * Get the Foreground color of the ContextMenu
     * @return the Color
     */
    public Color getForeground() {
        return ScilabBridge.getForeground(this);
    }

    /**
     * Set the font of the ContextMenu.
     * @param font the font
     */
    public void setFont(Font font) {
        ScilabBridge.setFont(this, font);
    }

    /**
     * Get the font of the ContextMenu.
     * @return the font
     */
    public Font getFont() {
        return ScilabBridge.getFont(this);
    }

    /**
     * Add a callback to the ContextMenu, this callback is a Scilab command
     * @param callback the CallBack to set.
     */
    public void setCallback(CommonCallBack callback) {
        ScilabBridge.setCallback(this, callback);
    }

    /**
     * Set the horizontal alignment for the ContextMenu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        ScilabBridge.setHorizontalAlignment(this, alignment);
    }

    /**
     * Set the vertical alignment for the ContextMenu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        ScilabBridge.setVerticalAlignment(this, alignment);
    }

    /**
     * Set the Relief of the ContextMenu
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        ScilabBridge.setRelief(this, reliefType);
    }

    /**
     * Delete the ContextMenu
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
