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

package org.scilab.modules.gui.label;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab PushButtons in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabLabelBridge {

	/**
	 * Constructor
	 */
	protected ScilabLabelBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Label
	 * @return the created Label
	 */
	public static SimpleLabel createLabel() {
		return new SwingScilabLabel();
	}
	
	/**
	 * Sets the Text of a Scilab Label
	 * @param label the Label we want to set the Text of
	 * @param newText the Text we want to set to the Label
	 */
	public static void setText(Label label, String newText) {
		label.getAsSimpleLabel().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab Label
	 * @param label the Label we want to get the Text of
	 * @return the text of the Label
	 */
	public static String getText(Label label) {
		return label.getAsSimpleLabel().getText();
	}
	
	/**
	 * Draws a Scilab label
	 * @param label the label to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(Label label) {
		label.getAsSimpleLabel().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Label
	 * @param label the label we want to get the dimensions of
	 * @return the size of the label
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(Label label) {
		return label.getAsSimpleLabel().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab label
	 * @param label the label we want to get the position of
	 * @return the position of the label
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(Label label) {
		return label.getAsSimpleLabel().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab Label
	 * @param label the label we want to get the visiblity status of
	 * @return the visibility status of the label (true if the label is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(Label label) {
		return label.getAsSimpleLabel().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Label
	 * @param label the label we want to set the dimensions of
	 * @param newSize the size we want to set to the label
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(Label label, Size newSize) {
		label.getAsSimpleLabel().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab label
	 * @param label the label we want to set the position of
	 * @param newPosition the position we want to set to the label
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(Label label, Position newPosition) {
		label.getAsSimpleLabel().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Label
	 * @param label the label we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the label (true to set the label visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Label label, boolean newVisibleState) {
		label.getAsSimpleLabel().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the Label
	 * @param label the Label we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(Label label, CallBack callback) {
		label.getAsSimpleLabel().setCallback(callback);
	}
	
	/**
	 * To set the Background color of the Label
	 * @param label the Label we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Label label, Color color) {
		label.getAsSimpleLabel().setBackground(color);
	}

	/**
	 * Get the Background color of the Label
	 * @param label the Label we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Label label) {
		return label.getAsSimpleLabel().getBackground();
	}

	/**
	 * To set the Foreground color of the label
	 * @param label the Label we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Label label, Color color) {
		label.getAsSimpleLabel().setForeground(color);
	}

	/**
	 * Get the Foreground color of the Label
	 * @param label the Label we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Label label) {
		return label.getAsSimpleLabel().getForeground();
	}

	/**
	 * Set the font of the Label.
	 * @param label the Label we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Label label, Font font) {
		label.getAsSimpleLabel().setFont(font);
	}
	
	/**
	 * Get the font of the Label.
	 * @param label the Label we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Label label) {
		return label.getAsSimpleLabel().getFont();
	}
	
	/**
	 * Set if the Label is enabled or not
	 * @param label the Label we want to set the status of
	 * @param status true if the Label is enabled
	 */
	public static void setEnabled(Label label, boolean status) {
		label.getAsSimpleLabel().setEnabled(status);
	}
	
	/**
	 * Gets the enable status of a Label
	 * @param label the Label we want to get the status of
	 * @return the enable status of the Label (true if the Label is enabled, false if not)
	 */
	public static boolean isEnabled(Label label) {
		return label.getAsSimpleLabel().isEnabled();
	}
	
	/**
	 * Set the horizontal alignment for the Label text
	 * @param label the Label we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(Label label, String alignment) {
		label.getAsSimpleLabel().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the Label text
	 * @param label the Label we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(Label label, String alignment) {
		label.getAsSimpleLabel().setVerticalAlignment(alignment);
	}

	/**
	 * Set the Relief of the Label
	 * @param label the Label which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Label label, String reliefType) {
		label.getAsSimpleLabel().setRelief(reliefType);
	}

	/**
	 * Delete the Label
	 * @param label the Label to be destroyed
	 */
	public static void destroy(Label label) {
		label.getAsSimpleLabel().destroy();
	}
	
	/**
	 * Give the focus to the Label
	 * @param label the Label
	 */
	public static void requestFocus(Label label) {
		label.getAsSimpleLabel().requestFocus();
	}

}
