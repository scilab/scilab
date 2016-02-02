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

import org.scilab.modules.gui.bridge.uidisplaytree.SwingScilabUiDisplayTree;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab UiDisplayTrees in GUIs
 * @author Han DONG
 */
public class ScilabUiDisplayTreeBridge {

    /**
     * Constructor
     */
    protected ScilabUiDisplayTreeBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab UiDisplayTree
     * @return the created UiDisplayTree
     */
    public static SimpleUiDisplayTree createUiDisplayTree() {
        return new SwingScilabUiDisplayTree();
    }

    /**
     * Sets the Text of a Scilab UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the Text of
     * @param newText the Text we want to set to the UiDisplayTree
     */
    public static void setText(UiDisplayTree uiTree, String newText) {
        uiTree.getAsSimpleUiDisplayTree().setText(newText);
    }

    /**
     * Sets the Text of a Scilab UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the Text of
     * @return the text of the UiDisplayTree
     */
    public static String getText(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().getText();
    }

    /**
     * Draws a Scilab uiTree
     * @param uiTree the uiTree to draw
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public static void draw(UiDisplayTree uiTree) {
        uiTree.getAsSimpleUiDisplayTree().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to get the dimensions of
     * @return the size of the uiTree
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public static Size getDims(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab uiTree
     * @param uiTree the uiTree we want to get the position of
     * @return the position of the uiTree
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public static Position getPosition(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().getPosition();
    }

    /**
     * Gets the visibility status of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to get the visiblity status of
     * @return the visibility status of the uiTree (true if the uiTree is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to set the dimensions of
     * @param newSize the size we want to set to the uiTree
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public static void setDims(UiDisplayTree uiTree, Size newSize) {
        uiTree.getAsSimpleUiDisplayTree().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab uiTree
     * @param uiTree the uiTree we want to set the position of
     * @param newPosition the position we want to set to the uiTree
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public static void setPosition(UiDisplayTree uiTree, Position newPosition) {
        uiTree.getAsSimpleUiDisplayTree().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the uiTree (true to set the uiTree visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(UiDisplayTree uiTree, boolean newVisibleState) {
        uiTree.getAsSimpleUiDisplayTree().setVisible(newVisibleState);
    }

    /**
     * Add a callback to the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(UiDisplayTree uiTree, CommonCallBack callback) {
        uiTree.getAsSimpleUiDisplayTree().setCallback(callback);
    }

    /**
     * To set the Background color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the background of
     * @param color the Color
     */
    public static void setBackground(UiDisplayTree uiTree, Color color) {
        uiTree.getAsSimpleUiDisplayTree().setBackground(color);
    }

    /**
     * Get the Background color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the background of
     * @return the Color
     */
    public static Color getBackground(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().getBackground();
    }

    /**
     * To set the Foreground color of the uiTree
     * @param uiTree the UiDisplayTree we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(UiDisplayTree uiTree, Color color) {
        uiTree.getAsSimpleUiDisplayTree().setForeground(color);
    }

    /**
     * Get the Foreground color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().getForeground();
    }

    /**
     * Set the font of the UiDisplayTree.
     * @param uiTree the UiDisplayTree we want to set the font of
     * @param font the font
     */
    public static void setFont(UiDisplayTree uiTree, Font font) {
        uiTree.getAsSimpleUiDisplayTree().setFont(font);
    }

    /**
     * Get the font of the UiDisplayTree.
     * @param uiTree the UiDisplayTree we want to get the font of
     * @return the font
     */
    public static Font getFont(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().getFont();
    }

    /**
     * Set if the UiDisplayTree is enabled or not
     * @param uiTree the UiDisplayTree we want to set the status of
     * @param status true if the UiDisplayTree is enabled
     */
    public static void setEnabled(UiDisplayTree uiTree, boolean status) {
        uiTree.getAsSimpleUiDisplayTree().setEnabled(status);
    }

    /**
     * Gets the enable status of a UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the status of
     * @return the enable status of the UiDisplayTree (true if the UiDisplayTree is enabled, false if not)
     */
    public static boolean isEnabled(UiDisplayTree uiTree) {
        return uiTree.getAsSimpleUiDisplayTree().isEnabled();
    }

    /**
     * Set the horizontal alignment for the UiDisplayTree text
     * @param uiTree the UiDisplayTree we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(UiDisplayTree uiTree, String alignment) {
        uiTree.getAsSimpleUiDisplayTree().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the UiDisplayTree text
     * @param uiTree the UiDisplayTree we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(UiDisplayTree uiTree, String alignment) {
        uiTree.getAsSimpleUiDisplayTree().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the UiDisplayTree
     * @param uiTree the UiDisplayTree which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(UiDisplayTree uiTree, String reliefType) {
        uiTree.getAsSimpleUiDisplayTree().setRelief(reliefType);
    }

    /**
     * Delete the UiDisplayTree
     * @param uiTree the UiDisplayTree to be destroyed
     */
    public static void destroy(UiDisplayTree uiTree) {
        uiTree.getAsSimpleUiDisplayTree().destroy();
    }

    /**
     * Give the focus to the UiDisplayTree
     * @param uiTree the UiDisplayTree
     */
    public static void requestFocus(UiDisplayTree uiTree) {
        uiTree.getAsSimpleUiDisplayTree().requestFocus();
    }

    /**
     * Sets the tree data to be added to viewport
     * @param text the array of strings containing tree data
     */
    public static void setData(UiDisplayTree uiTree, String[] text) {
        uiTree.getAsSimpleUiDisplayTree().setData(text);
    }
}
