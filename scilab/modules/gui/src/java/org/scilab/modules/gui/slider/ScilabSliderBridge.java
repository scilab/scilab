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

package org.scilab.modules.gui.slider;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab Sliders in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabSliderBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabSliderBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Slider
	 * @return the created Slider
	 */
	public static SimpleSlider createSlider() {
		return new SwingScilabSlider();
	}
	
	/**
	 * Sets the Text of a Scilab Slider
	 * @param slider the Slider we want to set the Text of
	 * @param newText the Text we want to set to the Slider
	 */
	public static void setText(Slider slider, String newText) {
		slider.getAsSimpleSlider().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab Slider
	 * @param slider the Slider we want to get the Text of
	 * @return the text of the Slider
	 */
	public static String getText(Slider slider) {
		return slider.getAsSimpleSlider().getText();
	}
	
	/**
	 * Draws a Scilab Slider
	 * @param slider the Slider to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(Slider slider) {
		slider.getAsSimpleSlider().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Slider
	 * @param slider the Slider we want to get the dimensions of
	 * @return the size of the Slider
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(Slider slider) {
		return slider.getAsSimpleSlider().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab Slider
	 * @param slider the Slider we want to get the position of
	 * @return the position of the Slider
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(Slider slider) {
		return slider.getAsSimpleSlider().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab Slider
	 * @param slider the Slider we want to get the visiblity status of
	 * @return the visibility status of the Slider (true if the Slider is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(Slider slider) {
		return slider.getAsSimpleSlider().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Slider
	 * @param slider the Slider we want to set the dimensions of
	 * @param newSize the size we want to set to the Slider
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(Slider slider, Size newSize) {
		slider.getAsSimpleSlider().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab Slider
	 * @param slider the Slider we want to set the position of
	 * @param newPosition the position we want to set to the Slider
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(Slider slider, Position newPosition) {
		slider.getAsSimpleSlider().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Slider
	 * @param slider the Slider we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the Slider (true to set the Slider visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Slider slider, boolean newVisibleState) {
		slider.getAsSimpleSlider().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the Slider
	 * @param slider the Slider we want to set the callback of
	 * @param callback the callback to set.
	 */
	public static void setCallback(Slider slider, CallBack callback) {
		slider.getAsSimpleSlider().setCallback(callback);
	}
	
	/**
	 * To set the Background color of the Slider
	 * @param slider the Slider we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Slider slider, Color color) {
		slider.getAsSimpleSlider().setBackground(color);
	}

	/**
	 * Get the Background color of the Slider
	 * @param slider the Slider we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Slider slider) {
		return slider.getAsSimpleSlider().getBackground();
	}

	/**
	 * To set the Foreground color of the Slider
	 * @param slider the Slider we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Slider slider, Color color) {
		slider.getAsSimpleSlider().setForeground(color);
	}

	/**
	 * Get the Foreground color of the Slider
	 * @param slider the Slider we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Slider slider) {
		return slider.getAsSimpleSlider().getForeground();
	}

	/**
	 * Set the font of the Slider.
	 * @param slider the Slider we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Slider slider, Font font) {
		slider.getAsSimpleSlider().setFont(font);
	}
	
	/**
	 * Get the font of the Slider.
	 * @param slider the Slider we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Slider slider) {
		return slider.getAsSimpleSlider().getFont();
	}
	
	/**
	 * Set if the Slider is enabled or not
	 * @param slider the Slider we want to set the status of
	 * @param status true if the Slider is enabled
	 */
	public static void setEnabled(Slider slider, boolean status) {
		slider.getAsSimpleSlider().setEnabled(status);
	}

	/**
	 * Gets the enable status of an Slider
	 * @param slider the Slider we want to get the status of
	 * @return the enable status of the Slider (true if the Slider is enabled, false if not)
	 */
	public static boolean isEnabled(Slider slider) {
		return slider.getAsSimpleSlider().isEnabled();
	}
	
	/**
	 * Set the horizontal alignment for the Slider text
	 * @param slider the Slider we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(Slider slider, String alignment) {
		slider.getAsSimpleSlider().setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the Slider text
	 * @param slider the Slider we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(Slider slider, String alignment) {
		slider.getAsSimpleSlider().setVerticalAlignment(alignment);
	}

	/**
	 * Set the minor tick spacing for a Slider
	 * @param slider the Slider we want to set the spacing of
	 * @param space the increment value
	 */
	public static void setMinorTickSpacing(Slider slider, int space) {
		slider.getAsSimpleSlider().setMinorTickSpacing(space);	
	}

	/**
	 * Set the major tick spacing for a Slider
	 * @param slider the Slider we want to set the spacing of
	 * @param space the increment value
	 */
	public static void setMajorTickSpacing(Slider slider, int space) {
		slider.getAsSimpleSlider().setMajorTickSpacing(space);	
	}

	/**
	 * Set the minimum value of a Slider
	 * @param slider the Slider we want to set the minimum value of
	 * @param value the minimum value
	 */
	public static void setMinimumValue(Slider slider, int value) {
		slider.getAsSimpleSlider().setMinimumValue(value);	
	}

	/**
	 * Set the maximum value of a Slider
	 * @param slider the Slider we want to set the maximum value of
	 * @param value the maximum value
	 */
	public static void setMaximumValue(Slider slider, int value) {
		slider.getAsSimpleSlider().setMaximumValue(value);	
	}

	/**
	 * Get the current value of the Slider
	 * @param slider the Slider we want to get the value of
	 * @return the current value of the Slider
	 */
	public static int getValue(Slider slider) {
		return slider.getAsSimpleSlider().getValue();	
	}
	
	/**
	 * Sets the current value of the Slider
	 * @param slider the Slider we want to set the value of
	 * @param value the new value
	 */
	public static void setUserValue(Slider slider, int value) {
		slider.getAsSimpleSlider().setUserValue(value);	
	}

	/**
	 * Set the Relief of the Slider
	 * @param slider the Slider which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Slider slider, String reliefType) {
		slider.getAsSimpleSlider().setRelief(reliefType);
	}

	/**
	 * Set the slider orientation to vertical
	 * @param slider the slider we want to set the orientation of
	 */
	public static void setVertical(Slider slider) {
		slider.getAsSimpleSlider().setVertical();
	}

	/**
	 * Set the slider orientation to horizontal
	 * @param slider the slider we want to set the orientation of
	 */
	public static void setHorizontal(Slider slider) {
		slider.getAsSimpleSlider().setHorizontal();
	}

	/**
	 * Delete the Slider
	 * @param slider the Slider to be destroyed
	 */
	public static void destroy(Slider slider) {
		slider.getAsSimpleSlider().destroy();
	}
	
	/**
	 * Give the focus to the Slider
	 * @param slider the Slider
	 */
	public static void requestFocus(Slider slider) {
		slider.getAsSimpleSlider().requestFocus();
	}

}
