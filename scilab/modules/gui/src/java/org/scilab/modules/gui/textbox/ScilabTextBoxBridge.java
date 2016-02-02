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

package org.scilab.modules.gui.textbox;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab TextBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabTextBoxBridge {

    /**
     * Constructor
     */
    protected ScilabTextBoxBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab TextBox
     * @return the created TextBox
     */
    public static SimpleTextBox createTextBox() {
        return new SwingScilabTextBox();
    }

    /**
     * Sets the Text of a Scilab TextBox
     * @param textBox the TextBox we want to set the Text of
     * @param newText the Text we want to set to the TextBox
     */
    public static void setText(TextBox textBox, String newText) {
        textBox.getAsSimpleTextBox().setText(newText);
    }

    /**
     * Sets the Text of a Scilab TextBox
     * @param textBox the TextBox we want to get the Text of
     * @return the text of the TextBox
     */
    public static String getText(TextBox textBox) {
        return textBox.getAsSimpleTextBox().getText();
    }

    /**
     * Draws a Scilab textBox
     * @param textBox the textBox to draw
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public static void draw(TextBox textBox) {
        textBox.getAsSimpleTextBox().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab TextBox
     * @param textBox the textBox we want to get the dimensions of
     * @return the size of the textBox
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public static Size getDims(TextBox textBox) {
        return textBox.getAsSimpleTextBox().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab textBox
     * @param textBox the textBox we want to get the position of
     * @return the position of the textBox
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public static Position getPosition(TextBox textBox) {
        return textBox.getAsSimpleTextBox().getPosition();
    }

    /**
     * Gets the visibility status of a Scilab TextBox
     * @param textBox the textBox we want to get the visiblity status of
     * @return the visibility status of the textBox (true if the textBox is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(TextBox textBox) {
        return textBox.getAsSimpleTextBox().isVisible();
    }

    /**
     * Set the status of the TextBox
     * @param textBox the textBox we want to set the status of
     * @param status true if the TextBox is enabled
     * @see org.scilab.modules.gui.widget.Widget#setEnabled(boolean)
     */
    public static void setEnabled(TextBox textBox, boolean status) {
        textBox.getAsSimpleTextBox().setEnabled(status);
    }

    /**
     * Gets the enable status of a TextBox
     * @param textBox the textBox we want to get the status of
     * @return the enable status of the TextBox (true if the TextBox is enabled, false if not)
     */
    public static boolean isEnabled(TextBox textBox) {
        return textBox.getAsSimpleTextBox().isEnabled();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab TextBox
     * @param textBox the textBox we want to set the dimensions of
     * @param newSize the size we want to set to the textBox
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public static void setDims(TextBox textBox, Size newSize) {
        textBox.getAsSimpleTextBox().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab textBox
     * @param textBox the textBox we want to set the position of
     * @param newPosition the position we want to set to the textBox
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public static void setPosition(TextBox textBox, Position newPosition) {
        textBox.getAsSimpleTextBox().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab TextBox
     * @param textBox the textBox we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the textBox (true to set the textBox visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(TextBox textBox, boolean newVisibleState) {
        textBox.getAsSimpleTextBox().setVisible(newVisibleState);
    }

    /**
     * Set the Background color of a Scilab TextBox
     * @param textBox the textbox we want to set the background of
     * @param color the Color
     */
    public static void setBackground(TextBox textBox, Color color) {
        textBox.getAsSimpleTextBox().setBackground(color);
    }

    /**
     * Get the Background color of the textbox
     * @param textBox the textbox we want to get the background of
     * @return the Color
     */
    public static Color getBackground(TextBox textBox) {
        return textBox.getAsSimpleTextBox().getBackground();
    }

    /**
     * Set the Foreground color of a Scilab TextBox
     * @param textBox the textbox we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(TextBox textBox, Color color) {
        textBox.getAsSimpleTextBox().setForeground(color);
    }

    /**
     * Get the Foreground color of the textbox
     * @param textBox the textbox we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(TextBox textBox) {
        return textBox.getAsSimpleTextBox().getForeground();
    }

    /**
     * Set the font of the textbox.
     * @param textBox the textbox we want to set the font of
     * @param font the font
     */
    public static void setFont(TextBox textBox, Font font) {
        textBox.getAsSimpleTextBox().setFont(font);
    }

    /**
     * Get the font of the textbox.
     * @param textBox the textbox we want to get the font of
     * @return the font
     */
    public static Font getFont(TextBox textBox) {
        return textBox.getAsSimpleTextBox().getFont();
    }

    /**
     * Add a callback to the TextBox
     * @param textBox the TextBox we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(TextBox textBox, CommonCallBack callback) {
        textBox.getAsSimpleTextBox().setCallback(callback);
    }

    /**
     * Set the horizontal alignment for the TextBox text
     * @param textBox the TextBox we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(TextBox textBox, String alignment) {
        textBox.getAsSimpleTextBox().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the TextBox text
     * @param textBox the TextBox we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(TextBox textBox, String alignment) {
        textBox.getAsSimpleTextBox().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the TextBox
     * @param textBox the TextBox which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(TextBox textBox, String reliefType) {
        textBox.getAsSimpleTextBox().setRelief(reliefType);
    }

    /**
     * Delete the TextBox
     * @param textBox the TextBox to be destroyed
     */
    public static void destroy(TextBox textBox) {
        textBox.getAsSimpleTextBox().destroy();
    }

    /**
     * Give the focus to the TextBox
     * @param textBox the TextBox
     */
    public static void requestFocus(TextBox textBox) {
        textBox.getAsSimpleTextBox().requestFocus();
    }

}
