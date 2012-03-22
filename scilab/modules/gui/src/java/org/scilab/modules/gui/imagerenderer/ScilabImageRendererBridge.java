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

package org.scilab.modules.gui.imagerenderer;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.imagerenderer.SwingScilabImageRenderer;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab ImageRenderers in GUIs
 * @author Han DONG
 */
public class ScilabImageRendererBridge {

    /**
     * Constructor
     */
    protected ScilabImageRendererBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab ImageRenderer
     * @return the created ImageRenderer
     */
    public static SimpleImageRenderer createImageRenderer() {
        return new SwingScilabImageRenderer();
    }

    /**
     * Sets the Text of a Scilab ImageRenderer
     * @param imageRender the ImageRenderer we want to set the Text of
     * @param newText the Text we want to set to the ImageRenderer
     */
    public static void setText(ImageRenderer imageRender, String newText) {
        imageRender.getAsSimpleImageRenderer().setText(newText);
    }

    /**
     * Sets the Text of a Scilab ImageRenderer
     * @param imageRender the ImageRenderer we want to get the Text of
     * @return the text of the ImageRenderer
     */
    public static String getText(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().getText();
    }

    /**
     * Draws a Scilab imageRender
     * @param imageRender the imageRender to draw
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public static void draw(ImageRenderer imageRender) {
        imageRender.getAsSimpleImageRenderer().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab ImageRenderer
     * @param imageRender the imageRender we want to get the dimensions of
     * @return the size of the imageRender
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public static Size getDims(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab imageRender
     * @param imageRender the imageRender we want to get the position of
     * @return the position of the imageRender
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public static Position getPosition(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().getPosition();
    }

    /**
     * Gets the visibility status of a Scilab ImageRenderer
     * @param imageRender the imageRender we want to get the visiblity status of
     * @return the visibility status of the imageRender (true if the imageRender is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab ImageRenderer
     * @param imageRender the imageRender we want to set the dimensions of
     * @param newSize the size we want to set to the imageRender
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public static void setDims(ImageRenderer imageRender, Size newSize) {
        imageRender.getAsSimpleImageRenderer().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab imageRender
     * @param imageRender the imageRender we want to set the position of
     * @param newPosition the position we want to set to the imageRender
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public static void setPosition(ImageRenderer imageRender, Position newPosition) {
        imageRender.getAsSimpleImageRenderer().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab ImageRenderer
     * @param imageRender the imageRender we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the imageRender (true to set the imageRender visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(ImageRenderer imageRender, boolean newVisibleState) {
        imageRender.getAsSimpleImageRenderer().setVisible(newVisibleState);
    }

    /**
     * Add a callback to the ImageRenderer
     * @param imageRender the ImageRenderer we want to set the callback of
     * @param callback the CallBack to set.
     */
    public static void setCallback(ImageRenderer imageRender, CommonCallBack callback) {
        imageRender.getAsSimpleImageRenderer().setCallback(callback);
    }

    /**
     * To set the Background color of the ImageRenderer
     * @param imageRender the ImageRenderer we want to set the background of
     * @param color the Color
     */
    public static void setBackground(ImageRenderer imageRender, Color color) {
        imageRender.getAsSimpleImageRenderer().setBackground(color);
    }

    /**
     * Get the Background color of the ImageRenderer
     * @param imageRender the ImageRenderer we want to get the background of
     * @return the Color
     */
    public static Color getBackground(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().getBackground();
    }

    /**
     * To set the Foreground color of the imageRender
     * @param imageRender the ImageRenderer we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(ImageRenderer imageRender, Color color) {
        imageRender.getAsSimpleImageRenderer().setForeground(color);
    }

    /**
     * Get the Foreground color of the ImageRenderer
     * @param imageRender the ImageRenderer we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().getForeground();
    }

    /**
     * Set the font of the ImageRenderer.
     * @param imageRender the ImageRenderer we want to set the font of
     * @param font the font
     */
    public static void setFont(ImageRenderer imageRender, Font font) {
        imageRender.getAsSimpleImageRenderer().setFont(font);
    }

    /**
     * Get the font of the ImageRenderer.
     * @param imageRender the ImageRenderer we want to get the font of
     * @return the font
     */
    public static Font getFont(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().getFont();
    }

    /**
     * Set if the ImageRenderer is enabled or not
     * @param imageRender the ImageRenderer we want to set the status of
     * @param status true if the ImageRenderer is enabled
     */
    public static void setEnabled(ImageRenderer imageRender, boolean status) {
        imageRender.getAsSimpleImageRenderer().setEnabled(status);
    }

    /**
     * Gets the enable status of a ImageRenderer
     * @param imageRender the ImageRenderer we want to get the status of
     * @return the enable status of the ImageRenderer (true if the ImageRenderer is enabled, false if not)
     */
    public static boolean isEnabled(ImageRenderer imageRender) {
        return imageRender.getAsSimpleImageRenderer().isEnabled();
    }

    /**
     * Set the horizontal alignment for the ImageRenderer text
     * @param imageRender the ImageRenderer we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(ImageRenderer imageRender, String alignment) {
        imageRender.getAsSimpleImageRenderer().setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the ImageRenderer text
     * @param imageRender the ImageRenderer we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(ImageRenderer imageRender, String alignment) {
        imageRender.getAsSimpleImageRenderer().setVerticalAlignment(alignment);
    }

    /**
     * Set the Relief of the ImageRenderer
     * @param imageRender the ImageRenderer which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(ImageRenderer imageRender, String reliefType) {
        imageRender.getAsSimpleImageRenderer().setRelief(reliefType);
    }

    /**
     * Delete the ImageRenderer
     * @param imageRender the ImageRenderer to be destroyed
     */
    public static void destroy(ImageRenderer imageRender) {
        imageRender.getAsSimpleImageRenderer().destroy();
    }

    /**
     * Give the focus to the ImageRenderer
     * @param imageRender the ImageRenderer
     */
    public static void requestFocus(ImageRenderer imageRender) {
        imageRender.getAsSimpleImageRenderer().requestFocus();
    }

    /**
     * Rotates an image
     * @param indices the double value for the angle to rotate
     */
    public static void setRotate(ImageRenderer imageRender, double[] indices) {
        // Commented out because will be removed
        //imageRender.getAsSimpleImageRenderer().setRotate(indices);
    }

    /**
     * Shears an image
     * @param indices the double array with x, y values to shear
     */
    public static void setShear(ImageRenderer imageRender, double[] indices) {
        imageRender.getAsSimpleImageRenderer().setShear(indices);
    }

    /**
     * Scaless an image
     * @param indices the double array with x, y values to scale
     */
    public static void setScale(ImageRenderer imageRender, double[] indices) {
        imageRender.getAsSimpleImageRenderer().setScale(indices);
    }
}
