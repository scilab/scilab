/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uitable;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.ScilabWidget;

/**
 * Swing implementation for Scilab UiTable in GUIs
 * @author Han DONG
 */
public class ScilabUiTable extends ScilabWidget implements UiTable {

    private SimpleUiTable component;

    /**
     * Constructor
     */
    protected ScilabUiTable() {
        component = ScilabBridge.createUiTable();
    }

    /**
     * Creates a Scilab UiTable object
     * @return the created UiTable
     */
    public static UiTable createUiTable() {
        return new ScilabUiTable();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleUiTable getAsSimpleUiTable() {
        return component;
    }

    /**
     * Sets the text of a UiTable
     * @param newText the text to set to the UiTable
     */
    public void setText(String newText) {
        ScilabBridge.setText(this, newText);
    }

    /**
     * Gets the text of a UiTable
     * @return text of the UiTable
     */
    public String getText() {
        return ScilabBridge.getText(this);
    }

    /**
     * Draws a UiTable
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
     * Add a callback to the UiTable
     * @param callback the CallBack to set.
     */
    public void setCallback(CommonCallBack callback) {
        ScilabBridge.setCallback(this, callback);
    }

    /**
     * Set if the UiTable is enabled or not
     * @param status true if the UiTable is enabled
     */
    public void setEnabled(boolean status) {
        ScilabBridge.setEnabled(this, status);
    }

    /**
     * Gets the enable status of a UiTable
     * @return the enable status of the UiTable (true if the UiTable is enabled, false if not)
     */
    public boolean isEnabled() {
        return ScilabBridge.isEnabled(this);
    }

    /**
     * Set the horizontal alignment for the UiTable text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        ScilabBridge.setHorizontalAlignment(this, alignment);
    }

    /**
     * Set the vertical alignment for the UiTable text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        ScilabBridge.setVerticalAlignment(this, alignment);
    }

    /**
     * Set the Relief of the UiTable
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        ScilabBridge.setRelief(this, reliefType);
    }

    /**
     * Delete the UiTable
     */
    public void destroy() {
        ScilabBridge.destroy(this);
    }

    /**
     * Give the focus to the UiTable
     */
    public void requestFocus() {
        ScilabBridge.requestFocus(this);
    }

    /**
     * Sets the column names for uitable
     * @param text the String that contains column names delimited by "|"
     */
    public void setColnames(String text) {
        ScilabBridge.setColnames(this, text);
    }

    /**
     * Sets the column names for uitable
     * @param text the String that contains column names delimited by "|"
     */
    public void setColumnNames(String[] text) {
        //ScilabBridge.setColnames(this, text);
    }

    /**
     * Sets the row names for uitable
     * @param text the String that contains row name delimited by a "|"
     */
    public void setRownames(String text) {
        ScilabBridge.setRownames(this, text);
    }

    /**
     * Sets the row names for uitable
     * @param text the String that contains row name delimited by a "|"
     */
    public void setRowNames(String[] text) {
        //ScilabBridge.setRownames(this, text);
    }

    /**
     * Sets the data for uitable
     * @param text the String that contains row data delimited by a '|'
     *        and column data delimited by " ". Example: 1.26 3.47 | a b | d e | a b
     */
    public void setData(String text) {
        ScilabBridge.setData(this, text);
    }

    @Override
    public void setToolTipText(String tooltipText) {
        // TODO Auto-generated method stub
    }
}
