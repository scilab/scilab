/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.uitable;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab UiTables in GUIs
 * @author Han DONG
 */
public class ScilabUiTableBridge {

    /**
     * Constructor
     */
    protected ScilabUiTableBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab UiTable
     * @return the created UiTable
     */
    public static SimpleUiTable createUiTable() {
        return new SwingScilabUiTable();
    }

    /**
     * Sets the Text of a Scilab UiTable
     * @param uiTable the UiTable we want to set the Text of
     * @param newText the Text we want to set to the UiTable
     */
    public static void setText(UiTable uiTable, String newText) {
        uiTable.getAsSimpleUiTable().setText(newText);
    }

    /**
     * Sets the Text of a Scilab UiTable
     * @param uiTable the UiTable we want to get the Text of
     * @return the text of the UiTable
     */
    public static String getText(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().getText();
    }

    /**
     * Draws a Scilab uiTable
     * @param uiTable the uiTable to draw
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public static void draw(UiTable uiTable) {
        uiTable.getAsSimpleUiTable().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab UiTable
     * @param uiTable the uiTable we want to get the dimensions of
     * @return the size of the uiTable
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public static Size getDims(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab uiTable
     * @param uiTable the uiTable we want to get the position of
     * @return the position of the uiTable
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public static Position getPosition(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().getPosition();
    }

    /**
     * Gets the visibility status of a Scilab UiTable
     * @param uiTable the uiTable we want to get the visiblity status of
     * @return the visibility status of the uiTable (true if the uiTable is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab UiTable
     * @param uiTable the uiTable we want to set the dimensions of
     * @param newSize the size we want to set to the uiTable
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public static void setDims(UiTable uiTable, Size newSize) {
        uiTable.getAsSimpleUiTable().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab uiTable
     * @param uiTable the uiTable we want to set the position of
     * @param newPosition the position we want to set to the uiTable
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public static void setPosition(UiTable uiTable, Position newPosition) {
        uiTable.getAsSimpleUiTable().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab UiTable
     * @param uiTable the uiTable we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the uiTable (true to set the uiTable visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(UiTable uiTable, boolean newVisibleState) {
        uiTable.getAsSimpleUiTable().setVisible(newVisibleState);
    }

    /**
     * Add a callback to the UiTable
     * @param uiTable the UiTable we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(UiTable uiTable, CommonCallBack callback) {
        uiTable.getAsSimpleUiTable().setCallback(callback);
    }

    /**
     * To set the Background color of the UiTable
     * @param uiTable the UiTable we want to set the background of
     * @param color the Color
     */
    public static void setBackground(UiTable uiTable, Color color) {
        uiTable.getAsSimpleUiTable().setBackground(color);
    }

    /**
     * Get the Background color of the UiTable
     * @param uiTable the UiTable we want to get the background of
     * @return the Color
     */
    public static Color getBackground(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().getBackground();
    }

    /**
     * To set the Foreground color of the uiTable
     * @param uiTable the UiTable we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(UiTable uiTable, Color color) {
        uiTable.getAsSimpleUiTable().setForeground(color);
    }

    /**
     * Get the Foreground color of the UiTable
     * @param uiTable the UiTable we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().getForeground();
    }

    /**
     * Set the font of the UiTable.
     * @param uiTable the UiTable we want to set the font of
     * @param font the font
     */
    public static void setFont(UiTable uiTable, Font font) {
        uiTable.getAsSimpleUiTable().setFont(font);
    }

    /**
     * Get the font of the UiTable.
     * @param uiTable the UiTable we want to get the font of
     * @return the font
     */
    public static Font getFont(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().getFont();
    }

    /**
     * Set if the UiTable is enabled or not
     * @param uiTable the UiTable we want to set the status of
     * @param status true if the UiTable is enabled
     */
    public static void setEnabled(UiTable uiTable, boolean status) {
        uiTable.getAsSimpleUiTable().setEnabled(status);
    }

    /**
     * Gets the enable status of a UiTable
     * @param uiTable the UiTable we want to get the status of
     * @return the enable status of the UiTable (true if the UiTable is enabled, false if not)
     */
    public static boolean isEnabled(UiTable uiTable) {
        return uiTable.getAsSimpleUiTable().isEnabled();
    }

    /**
     * Set the horizontal alignment for the UiTable text
     * @param uiTable the UiTable we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(UiTable uiTable, String alignment) {
        uiTable.getAsSimpleUiTable().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the UiTable text
     * @param uiTable the UiTable we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(UiTable uiTable, String alignment) {
        uiTable.getAsSimpleUiTable().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the UiTable
     * @param uiTable the UiTable which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(UiTable uiTable, String reliefType) {
        uiTable.getAsSimpleUiTable().setRelief(reliefType);
    }

    /**
     * Delete the UiTable
     * @param uiTable the UiTable to be destroyed
     */
    public static void destroy(UiTable uiTable) {
        uiTable.getAsSimpleUiTable().destroy();
    }

    /**
     * Give the focus to the UiTable
     * @param uiTable the UiTable
     */
    public static void requestFocus(UiTable uiTable) {
        uiTable.getAsSimpleUiTable().requestFocus();
    }

    /**
     * Sets the column namesfor uitable
     * @param text the String that contains column names delimited by a '|'
     */
    public static void setColnames(UiTable uiTable, String text) {
        // TODO remove
        //uiTable.getAsSimpleUiTable().setColnames(text);
    }

    /**
     * Sets the row names for uitable
     * @param text the String that contains row name delimited by a '|'
     */
    public static void setRownames(UiTable uiTable, String text) {
        // TODO remove
        //uiTable.getAsSimpleUiTable().setRownames(text);
    }

    /**
     * Sets the data for uitable
     * @param text the String that contains row data delimited by a '|'
         *        and column data delimited by " ". Example: 1.26 3.47 | a b | d e | a b
     */
    public static void setData(UiTable uiTable, String text) {
        // TODO remove
        //uiTable.getAsSimpleUiTable().setData(text);
    }
}
