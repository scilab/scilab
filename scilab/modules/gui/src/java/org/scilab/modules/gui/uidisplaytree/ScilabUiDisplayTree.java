/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
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

package org.scilab.modules.gui.uidisplaytree;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;

/**
 * Swing implementation for Scilab UiDisplayTree in GUIs
 * @author Han DONG
 */
public class ScilabUiDisplayTree extends ScilabWidget implements UiDisplayTree {

    private SimpleUiDisplayTree component;

    /**
     * Constructor
     */
    protected ScilabUiDisplayTree() {
        component = ScilabBridge.createUiDisplayTree();
    }

    /**
     * Creates a Scilab UiDisplayTree object
     * @return the created UiDisplayTree
     */
    public static UiDisplayTree createUiDisplayTree() {
        return new ScilabUiDisplayTree();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleUiDisplayTree getAsSimpleUiDisplayTree() {
        return component;
    }

    /**
     * Sets the text of a UiDisplayTree
     * @param newText the text to set to the UiDisplayTree
     */
    public void setText(String newText) {
        ScilabBridge.setText(this, newText);
    }

    public void setEmptyText() {
        setText(null);
    }

    /**
     * Gets the text of a UiDisplayTree
     * @return text of the UiDisplayTree
     */
    public String getText() {
        return ScilabBridge.getText(this);
    }

    /**
     * Draws a UiDisplayTree
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
     * Add a callback to the UiDisplayTree
     * @param callback the CallBack to set.
     */
    public void setCallback(CommonCallBack callback) {
        ScilabBridge.setCallback(this, callback);
    }

    /**
     * Set if the UiDisplayTree is enabled or not
     * @param status true if the UiDisplayTree is enabled
     */
    public void setEnabled(boolean status) {
        ScilabBridge.setEnabled(this, status);
    }

    /**
     * Gets the enable status of a UiDisplayTree
     * @return the enable status of the UiDisplayTree (true if the UiDisplayTree is enabled, false if not)
     */
    public boolean isEnabled() {
        return ScilabBridge.isEnabled(this);
    }

    /**
     * Set the horizontal alignment for the UiDisplayTree text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        ScilabBridge.setHorizontalAlignment(this, alignment);
    }

    /**
     * Set the vertical alignment for the UiDisplayTree text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        ScilabBridge.setVerticalAlignment(this, alignment);
    }

    /**
     * Set the Relief of the UiDisplayTree
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        ScilabBridge.setRelief(this, reliefType);
    }

    /**
     * Delete the UiDisplayTree
     */
    public void destroy() {
        ScilabBridge.destroy(this);
    }

    /**
     * Give the focus to the UiDisplayTree
     */
    public void requestFocus() {
        ScilabBridge.requestFocus(this);
    }

    /**
     * Sets the tree data to be added to viewport
     * @param text the array of strings containing tree data
     */
    public void setData(String[] text) {
        ScilabBridge.setData(this, text);
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
