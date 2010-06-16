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

package org.scilab.modules.gui.imagerender;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.imagerender.SwingScilabImageRender;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab ImageRenders in GUIs
 * @author Han DONG
 */
public class ScilabImageRenderBridge {

	/**
	 * Constructor
	 */
	protected ScilabImageRenderBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab ImageRender
	 * @return the created ImageRender
	 */
	public static SimpleImageRender createImageRender() {
		return new SwingScilabImageRender();
	}
	
	/**
	 * Sets the Text of a Scilab ImageRender
	 * @param imageRender the ImageRender we want to set the Text of
	 * @param newText the Text we want to set to the ImageRender
	 */
	public static void setText(ImageRender imageRender, String newText) {
		imageRender.getAsSimpleImageRender().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab ImageRender
	 * @param imageRender the ImageRender we want to get the Text of
	 * @return the text of the ImageRender
	 */
	public static String getText(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().getText();
	}
	
	/**
	 * Draws a Scilab imageRender
	 * @param imageRender the imageRender to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(ImageRender imageRender) {
		imageRender.getAsSimpleImageRender().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab ImageRender
	 * @param imageRender the imageRender we want to get the dimensions of
	 * @return the size of the imageRender
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab imageRender
	 * @param imageRender the imageRender we want to get the position of
	 * @return the position of the imageRender
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab ImageRender
	 * @param imageRender the imageRender we want to get the visiblity status of
	 * @return the visibility status of the imageRender (true if the imageRender is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab ImageRender
	 * @param imageRender the imageRender we want to set the dimensions of
	 * @param newSize the size we want to set to the imageRender
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(ImageRender imageRender, Size newSize) {
		imageRender.getAsSimpleImageRender().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab imageRender
	 * @param imageRender the imageRender we want to set the position of
	 * @param newPosition the position we want to set to the imageRender
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(ImageRender imageRender, Position newPosition) {
		imageRender.getAsSimpleImageRender().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab ImageRender
	 * @param imageRender the imageRender we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the imageRender (true to set the imageRender visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(ImageRender imageRender, boolean newVisibleState) {
		imageRender.getAsSimpleImageRender().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the ImageRender
	 * @param imageRender the ImageRender we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(ImageRender imageRender, CallBack callback) {
		imageRender.getAsSimpleImageRender().setCallback(callback);
	}
	
	/**
	 * To set the Background color of the ImageRender
	 * @param imageRender the ImageRender we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(ImageRender imageRender, Color color) {
		imageRender.getAsSimpleImageRender().setBackground(color);
	}

	/**
	 * Get the Background color of the ImageRender
	 * @param imageRender the ImageRender we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().getBackground();
	}

	/**
	 * To set the Foreground color of the imageRender
	 * @param imageRender the ImageRender we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(ImageRender imageRender, Color color) {
		imageRender.getAsSimpleImageRender().setForeground(color);
	}

	/**
	 * Get the Foreground color of the ImageRender
	 * @param imageRender the ImageRender we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().getForeground();
	}

	/**
	 * Set the font of the ImageRender.
	 * @param imageRender the ImageRender we want to set the font of
	 * @param font the font
	 */
	public static void setFont(ImageRender imageRender, Font font) {
		imageRender.getAsSimpleImageRender().setFont(font);
	}
	
	/**
	 * Get the font of the ImageRender.
	 * @param imageRender the ImageRender we want to get the font of
	 * @return the font
	 */
	public static Font getFont(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().getFont();
	}
	
	/**
	 * Set if the ImageRender is enabled or not
	 * @param imageRender the ImageRender we want to set the status of
	 * @param status true if the ImageRender is enabled
	 */
	public static void setEnabled(ImageRender imageRender, boolean status) {
		imageRender.getAsSimpleImageRender().setEnabled(status);
	}
	
	/**
	 * Gets the enable status of a ImageRender
	 * @param imageRender the ImageRender we want to get the status of
	 * @return the enable status of the ImageRender (true if the ImageRender is enabled, false if not)
	 */
	public static boolean isEnabled(ImageRender imageRender) {
		return imageRender.getAsSimpleImageRender().isEnabled();
	}
	
	/**
	 * Set the horizontal alignment for the ImageRender text
	 * @param imageRender the ImageRender we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(ImageRender imageRender, String alignment) {
		imageRender.getAsSimpleImageRender().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the ImageRender text
	 * @param imageRender the ImageRender we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(ImageRender imageRender, String alignment) {
		imageRender.getAsSimpleImageRender().setVerticalAlignment(alignment);
	}

	/**
	 * Set the Relief of the ImageRender
	 * @param imageRender the ImageRender which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(ImageRender imageRender, String reliefType) {
		imageRender.getAsSimpleImageRender().setRelief(reliefType);
	}

	/**
	 * Delete the ImageRender
	 * @param imageRender the ImageRender to be destroyed
	 */
	public static void destroy(ImageRender imageRender) {
		imageRender.getAsSimpleImageRender().destroy();
	}
	
	/**
	 * Give the focus to the ImageRender
	 * @param imageRender the ImageRender
	 */
	public static void requestFocus(ImageRender imageRender) {
		imageRender.getAsSimpleImageRender().requestFocus();
	}

	/**
	 * Rotates an image
	 * @param indices the double value for the angle to rotate
	 */
	public static void setRotate(ImageRender imageRender, double[] indices) {
		imageRender.getAsSimpleImageRender().setRotate(indices);		
	}

	/**
	 * Shears an image
	 * @param indices the double array with x, y values to shear
	 */
 	public static void setShear(ImageRender imageRender, double[] indices) {
		imageRender.getAsSimpleImageRender().setShear(indices);		
	}

	/**
	 * Scaless an image
	 * @param indices the double array with x, y values to scale
	 */
 	public static void setScale(ImageRender imageRender, double[] indices) {
		imageRender.getAsSimpleImageRender().setScale(indices);		
	}
}
