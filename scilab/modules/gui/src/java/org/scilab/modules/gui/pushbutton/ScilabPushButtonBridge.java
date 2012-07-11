/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.pushbutton;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab PushButtons in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabPushButtonBridge {

	/**
	 * Constructor
	 */
	protected ScilabPushButtonBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new Scilab PushButton
	 * @return the created PushButton
	 */
	public static SimplePushButton createPushButton() {
		return new SwingScilabPushButton();
	}

	/**
	 * Draws a Scilab pushButton
	 * @param pushButton the pushButton to draw
	 */
	public static void draw(PushButton pushButton) {
		pushButton.getAsSimplePushButton().draw();
	}
	/**
	 * Gets the dimensions (width and height) of a Scilab pushButton
	 * @param pushButton the pushButton we want to get the dimensions of
	 * @return the dimensions of the pushButton
	 */
	public static Size getDims(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getDims();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the dimensions of
	 * @param newSize the dimensions we want to set to the pushButton
	 */
	public static void setDims(PushButton pushButton, Size newSize) {
		pushButton.getAsSimplePushButton().setDims(newSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab pushButton
	 * @param pushButton the pushButton we want to get the position of
	 * @return the position of the pushButton
	 */

	public static Position getPosition(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getPosition();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the position of
	 * @param newPosition the position to set to the pushButton
	 */
	public static void setPosition(PushButton pushButton, Position newPosition) {
		pushButton.getAsSimplePushButton().setPosition(newPosition);
	}

	/**
	 * Gets the visibility status of a Scilab pushButton
	 * @param pushButton the pushButton we want to getthe visibility status of
	 * @return the visibility status of the pushButton (true if the pushButton is visible, false if not)
	 */
	public static boolean isVisible(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().isVisible();
	}

	/**
	 * Sets the visibility status of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the pushButton (true to set the pushButton visible, false else)
	 */
	public static void setVisible(PushButton pushButton, boolean newVisibleState) {
		pushButton.getAsSimplePushButton().setVisible(newVisibleState);
	}
	
	/**
	 * Sets the Text of a Scilab PushButton
	 * @param pushButton the PushButton we want to set the Text of
	 * @param newText the Text we want to set to the PushButton
	 */
	public static void setText(PushButton pushButton, String newText) {
		pushButton.getAsSimplePushButton().setText(newText);
	}

	/**
	 * Gets the text of a PushButton
	 * @param pushButton the PushButton we want to get the Text of
	 * @return the text of the PushButton
	 */
	public static String getText(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getBaseText();
	}
	
	/**
	 * Sets the icon of a PushButton
	 * @param pushButton the PushButton we want to set the icon of
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public static void setIcon(PushButton pushButton, String filename) {
		pushButton.getAsSimplePushButton().setIcon(filename);
	}

	/**
	 * Add a callback to the pushbutton
	 * @param pushButton the PushButton we want to set the callback of
	 * @param callback the callback to set.
	 */
	public static void setCallback(PushButton pushButton, CommonCallBack callback) {
		pushButton.getAsSimplePushButton().setCallback(callback);
	}

	/**
	 * Set if the pushbutton is enabled or not
	 * @param pushButton the PushButton we want to set the status of
	 * @param status true if the pushbutton is enabled
	 */
	public static void setEnabled(PushButton pushButton, boolean status) {
		pushButton.getAsSimplePushButton().setEnabled(status);
	}

	/**
	 * Gets the enable status of a PushButton
	 * @param pushButton the PushButton we want to get the status of
	 * @return the enable status of the PushButton (true if the PushButton is enabled, false if not)
	 */
	public static boolean isEnabled(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().isEnabled();
	}
	
	/**
	 * To set the Background color of the pushbutton
	 * @param pushButton the PushButton we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(PushButton pushButton, Color color) {
		pushButton.getAsSimplePushButton().setBackground(color);
	}

	/**
	 * Get the Background color of the pushbutton
	 * @param pushButton the PushButton we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getBackground();
	}

	/**
	 * To set the Foreground color of the pushbutton
	 * @param pushButton the PushButton we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(PushButton pushButton, Color color) {
		pushButton.getAsSimplePushButton().setForeground(color);
	}

	/**
	 * Get the Foreground color of the pushbutton
	 * @param pushButton the PushButton we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getForeground();
	}

	/**
	 * Set the font of the pushbutton.
	 * @param pushButton the pushbutton we want to set the font of
	 * @param font the font
	 */
	public static void setFont(PushButton pushButton, Font font) {
		pushButton.getAsSimplePushButton().setFont(font);
	}
	
	/**
	 * Get the font of the pushbutton.
	 * @param pushButton the pushbutton we want to get the font of
	 * @return the font
	 */
	public static Font getFont(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getFont();
	}

	/**
	 * Set the horizontal alignment for the PushButton text
	 * @param pushButton the PushButton we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(PushButton pushButton, String alignment) {
		pushButton.getAsSimplePushButton().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the PushButton text
	 * @param pushButton the PushButton we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(PushButton pushButton, String alignment) {
		pushButton.getAsSimplePushButton().setVerticalAlignment(alignment);
	}

	/**
	 * Set the Relief of the PushButton
	 * @param pushButton the PushButton which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(PushButton pushButton, String reliefType) {
		pushButton.getAsSimplePushButton().setRelief(reliefType);
	}

	/**
	 * Delete the PushButton
	 * @param pushButton the PushButton to be destroyed
	 */
	public static void destroy(PushButton pushButton) {
		pushButton.getAsSimplePushButton().destroy();
	}
	
	/**
	 * Set the string to be displayed when the mouse cursor is over the PushButton
	 * @param pushButton the PushButton
	 * @param toolTipText the string to set
	 */
	public static void setToolTipText(PushButton pushButton, String toolTipText) {
		pushButton.getAsSimplePushButton().setToolTipText(toolTipText);
	}
	
	/**
	 * Give the focus to the PushButton
	 * @param pushButton the PushButton
	 */
	public static void requestFocus(PushButton pushButton) {
		pushButton.getAsSimplePushButton().requestFocus();
	}

}

