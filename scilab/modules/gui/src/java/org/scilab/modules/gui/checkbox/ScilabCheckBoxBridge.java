/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.checkbox;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab TextBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabCheckBoxBridge {

	/**
	 * Constructor
	 */
	protected ScilabCheckBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab CheckBox
	 * @return the created CheckBox
	 */
	public static SimpleCheckBox createCheckBox() {
		return new SwingScilabCheckBox();
	}
	
	/**
	 * Sets the Text of a Scilab CheckBox
	 * @param checkBox the CheckBox we want to set the Text of
	 * @param newText the Text we want to set to the CheckBox
	 */
	public static void setText(CheckBox checkBox, String newText) {
		checkBox.getAsSimpleCheckBox().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab CheckBox
	 * @param checkBox the CheckBox we want to get the Text of
	 * @return the text of the CheckBox
	 */
	public static String getText(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().getText();
	}
	
	/**
	 * Draws a Scilab checkBox
	 * @param checkBox the checkBox to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(CheckBox checkBox) {
		checkBox.getAsSimpleCheckBox().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab CheckBox
	 * @param checkBox the checkBox we want to get the dimensions of
	 * @return the size of the checkBox
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab checkBox
	 * @param checkBox the checkBox we want to get the position of
	 * @return the position of the checkBox
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab CheckBox
	 * @param checkBox the checkBox we want to get the visiblity status of
	 * @return the visibility status of the checkBox (true if the checkBox is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab CheckBox
	 * @param checkBox the checkBox we want to set the dimensions of
	 * @param newSize the size we want to set to the checkBox
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(CheckBox checkBox, Size newSize) {
		checkBox.getAsSimpleCheckBox().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab checkBox
	 * @param checkBox the checkBox we want to set the position of
	 * @param newPosition the position we want to set to the checkBox
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(CheckBox checkBox, Position newPosition) {
		checkBox.getAsSimpleCheckBox().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab CheckBox
	 * @param checkBox the checkBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the checkBox (true to set the checkBox visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(CheckBox checkBox, boolean newVisibleState) {
		checkBox.getAsSimpleCheckBox().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the CheckBox
	 * @param checkBox the CheckBox we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(CheckBox checkBox, CommonCallBack callback) {
		checkBox.getAsSimpleCheckBox().setCallback(callback);
	}
	
	/**
	 * To set the Background color of the CheckBox
	 * @param checkBox the CheckBox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(CheckBox checkBox, Color color) {
		checkBox.getAsSimpleCheckBox().setBackground(color);
	}

	/**
	 * Get the Background color of the CheckBox
	 * @param checkBox the CheckBox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().getBackground();
	}

	/**
	 * To set the Foreground color of the checkBox
	 * @param checkBox the CheckBox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(CheckBox checkBox, Color color) {
		checkBox.getAsSimpleCheckBox().setForeground(color);
	}

	/**
	 * Get the Foreground color of the CheckBox
	 * @param checkBox the CheckBox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().getForeground();
	}

	/**
	 * Set the font of the CheckBox.
	 * @param checkBox the CheckBox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(CheckBox checkBox, Font font) {
		checkBox.getAsSimpleCheckBox().setFont(font);
	}
	
	/**
	 * Get the font of the CheckBox.
	 * @param checkBox the CheckBox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().getFont();
	}
	
	/**
	 * Set if the CheckBox is enabled or not
	 * @param checkBox the CheckBox we want to set the status of
	 * @param status true if the CheckBox is enabled
	 */
	public static void setEnabled(CheckBox checkBox, boolean status) {
		checkBox.getAsSimpleCheckBox().setEnabled(status);
	}
	
	/**
	 * Gets the enable status of the CheckBox
	 * @param checkBox the CheckBox we want to get the status of
	 * @return the enable status of the CheckBox (true if the CheckBox is enabled, false if not)
	 */
	public static boolean isEnabled(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().isEnabled();
	}
	
	/**
	 * Set the horizontal alignment for the CheckBox text
	 * @param checkBox the CheckBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(CheckBox checkBox, String alignment) {
		checkBox.getAsSimpleCheckBox().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the CheckBox text
	 * @param checkBox the CheckBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(CheckBox checkBox, String alignment) {
		checkBox.getAsSimpleCheckBox().setVerticalAlignment(alignment);
	}

	/**
	 * Set if the CheckBox is checked or not
	 * @param checkBox the CheckBox we want to set the status of
	 * @param status true to set the CheckBox checked
	 */
	public static void setChecked(CheckBox checkBox, boolean status) {
		checkBox.getAsSimpleCheckBox().setChecked(status);
	}
	
	/**
	 * Get the status of the CheckBox
	 * @param checkBox the CheckBox we want to get the status of
	 * @return true if the CheckBox is checked
	 */
	public static boolean isChecked(CheckBox checkBox) {
		return checkBox.getAsSimpleCheckBox().isChecked();
	}

	/**
	 * Set the Relief of the CheckBox
	 * @param checkBox the CheckBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(CheckBox checkBox, String reliefType) {
		checkBox.getAsSimpleCheckBox().setRelief(reliefType);
	}
	
	/**
	 * Delete the CheckBox
	 * @param checkBox the checkBox to be destroyed
	 */
	public static void destroy(CheckBox checkBox) {
		checkBox.getAsSimpleCheckBox().destroy();
	}
	
	/**
	 * Give the focus to the CheckBox
	 * @param checkBox the CheckBox
	 */
	public static void requestFocus(CheckBox checkBox) {
		checkBox.getAsSimpleCheckBox().requestFocus();
	}

}
