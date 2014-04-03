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

package org.scilab.modules.gui.pushbutton;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.events.callback.CommonCallBack;

/**
 * Class for Scilab PushButtons in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabPushButton extends ScilabWidget implements PushButton {

    private SimplePushButton component;

    /**
     * Constructor
     */
    protected ScilabPushButton() {
        component = ScilabBridge.createPushButton();
    }

    /**
     * Creates a Scilab PushButton object
     * @return the created PushButton
     */
    public static PushButton createPushButton() {
        return new ScilabPushButton();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimplePushButton getAsSimplePushButton() {
        return component;
    }

    /**
     * Sets the text of a PushButton
     * @param newText the text to set to the PushButton
     */
    public void setText(String newText) {
        ScilabBridge.setText(this, newText);
    }

    public void setEmptyText() {
        setText(null);
    }

    /**
     * Gets the text of a PushButton
     * @return the text of the PushButton
     */
    public String getText() {
        return ScilabBridge.getText(this);
    }

    /**
     * Draws a PushButton
     */
    public void draw() {
        ScilabBridge.draw(this);
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab PushButton
     * @param newSize the dimensions to set to the PushButton
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        ScilabBridge.setDims(this, newSize);
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab Menu
     * @return the dimensions (width and height) of the window
     * @see org.scilab.modules.gui.uielement.UIElement#getDims(org.scilab.modules.gui.utils.Size)
     */
    public Size getDims() {
        return ScilabBridge.getDims(this);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab Menu
     * @return the position of the Menu
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return ScilabBridge.getPosition(this);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab Menu
     * @param newPosition the position we want to set to the Menu
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
     * Sets the icon of a PushButton
     * @param filename the path to the icon image to set to the PushButton
     */
    public void setIcon(String filename) {
        ScilabBridge.setIcon(this, filename);
    }

    /**
     * Add a callback to the pushbutton, this callback is a Scilab command
     * @param callback the CallBack to set.
     */
    public void setCallback(CommonCallBack callback) {
        ScilabBridge.setCallback(this, callback);
    }

    /**
     * Set if the pushbutton is enabled or not
     * @param status true if the pushbutton is enabled
     */
    public void setEnabled(boolean status) {
        ScilabBridge.setEnabled(this, status);
    }

    /**
     * Gets the enable status of a PushButton
     * @return the enable status of the PushButton (true if the PushButton is enabled, false if not)
     */
    public boolean isEnabled() {
        return ScilabBridge.isEnabled(this);
    }

    /**
     * To set the Background color of the pushbutton
     * @param color the Color
     */
    public void setBackground(Color color) {
        ScilabBridge.setBackground(this, color);
    }

    /**
     * Get the Background color of the pushbutton
     * @return the Color
     */
    public Color getBackground() {
        return ScilabBridge.getBackground(this);
    }

    /**
     * To set the Foreground color of the pushbutton
     * @param color the Color
     */
    public void setForeground(Color color) {
        ScilabBridge.setForeground(this, color);
    }

    /**
     * Get the Foreground color of the pushbutton
     * @return the Color
     */
    public Color getForeground() {
        return ScilabBridge.getForeground(this);
    }

    /**
     * Set the font of the pushbutton.
     * @param font the font
     */
    public void setFont(Font font) {
        ScilabBridge.setFont(this, font);
    }

    /**
     * Get the font of the pushbutton.
     * @return the font
     */
    public Font getFont() {
        return ScilabBridge.getFont(this);
    }

    /**
     * Set the horizontal alignment for the PushButton text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        ScilabBridge.setHorizontalAlignment(this, alignment);
    }

    /**
     * Set the vertical alignment for the PushButton text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        ScilabBridge.setVerticalAlignment(this, alignment);
    }

    /**
     * Set the Relief of the PushButton
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        ScilabBridge.setRelief(this, reliefType);
    }

    /**
     * Delete the PushButton
     */
    public void destroy() {
        ScilabBridge.destroy(this);
    }

    /**
     * Set the string to be displayed when the mouse cursor is over the PushButton
     * @param toolTipText the string to set
     */
    public void setToolTipText(String toolTipText) {
        ScilabBridge.setToolTipText(this, toolTipText);
    }

    /**
     * Give the focus to the ContextMenu
     */
    public void requestFocus() {
        ScilabBridge.requestFocus(this);
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
