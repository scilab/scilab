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

package org.scilab.modules.gui.editbox;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab EditBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabEditBoxBridge {

	/**
	 * Constructor
	 */
	protected ScilabEditBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab EditBox
	 * @return the created EditBox
	 */
	public static SimpleEditBox createEditBox() {
		return new SwingScilabEditBox();
	}
	
	/**
	 * Sets the Text of a Scilab EditBox
	 * @param editBox the EditBox we want to set the Text of
	 * @param newText the Text we want to set to the EditBox
	 */
	public static void setText(EditBox editBox, String newText) {
		editBox.getAsSimpleEditBox().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab EditBox
	 * @param editBox the EditBox we want to get the Text of
	 * @return the text of the EditBox
	 */
	public static String getText(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getText();
	}
	
	/**
	 * Draws a Scilab editBox
	 * @param editBox the editBox to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(EditBox editBox) {
		editBox.getAsSimpleEditBox().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to get the dimensions of
	 * @return the size of the editBox
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to get the position of
	 * @return the position of the editBox
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to get the visiblity status of
	 * @return the visibility status of the editBox (true if the editBox is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(EditBox editBox) {
		return editBox.getAsSimpleEditBox().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to set the dimensions of
	 * @param newSize the size we want to set to the editBox
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(EditBox editBox, Size newSize) {
		editBox.getAsSimpleEditBox().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to set the position of
	 * @param newPosition the position we want to set to the editBox
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(EditBox editBox, Position newPosition) {
		editBox.getAsSimpleEditBox().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the editBox (true to set the editBox visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(EditBox editBox, boolean newVisibleState) {
		editBox.getAsSimpleEditBox().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the EditBox
	 * @param editBox the EditBox we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(EditBox editBox, CommonCallBack callback) {
		editBox.getAsSimpleEditBox().setCallback(callback);
	}
	
	/**
	 * To set the Background color of the EditBox
	 * @param editBox the EditBox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(EditBox editBox, Color color) {
		editBox.getAsSimpleEditBox().setBackground(color);
	}

	/**
	 * Get the Background color of the EditBox
	 * @param editBox the EditBox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getBackground();
	}

	/**
	 * To set the Foreground color of the editBox
	 * @param editBox the EditBox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(EditBox editBox, Color color) {
		editBox.getAsSimpleEditBox().setForeground(color);
	}

	/**
	 * Get the Foreground color of the EditBox
	 * @param editBox the EditBox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getForeground();
	}

	/**
	 * Set the font of the EditBox.
	 * @param editBox the EditBox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(EditBox editBox, Font font) {
		editBox.getAsSimpleEditBox().setFont(font);
	}
	
	/**
	 * Get the font of the EditBox.
	 * @param editBox the EditBox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getFont();
	}
	
	/**
	 * Set if the EditBox is enabled or not
	 * @param editBox the EditBox we want to set the status of
	 * @param status true if the EditBox is enabled
	 */
	public static void setEnabled(EditBox editBox, boolean status) {
		editBox.getAsSimpleEditBox().setEnabled(status);
	}

	/**
	 * Gets the enable status of an EditBox
	 * @param editBox the EditBox we want to get the status of
	 * @return the enable status of the EditBox (true if the EditBox is enabled, false if not)
	 */
	public static boolean isEnabled(EditBox editBox) {
		return editBox.getAsSimpleEditBox().isEnabled();
	}
	
	/**
	 * Set the horizontal alignment for the EditBox text
	 * @param editBox the EditBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(EditBox editBox, String alignment) {
		editBox.getAsSimpleEditBox().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the EditBox text
	 * @param editBox the EditBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(EditBox editBox, String alignment) {
		editBox.getAsSimpleEditBox().setVerticalAlignment(alignment);
	}

	/**
	 * Set the Relief of the EditBox
	 * @param editBox the EditBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(EditBox editBox, String reliefType) {
		editBox.getAsSimpleEditBox().setRelief(reliefType);
	}

	/**
	 * Delete the EditBox
	 * @param editBox the EditBox to be destroyed
	 */
	public static void destroy(EditBox editBox) {
		editBox.getAsSimpleEditBox().destroy();
	}
	
	/**
	 * Give the focus to the EditBox
	 * @param editBox the EditBox
	 */
	public static void requestFocus(EditBox editBox) {
		editBox.getAsSimpleEditBox().requestFocus();
	}

}
