
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.slider;

import javax.swing.JSlider;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.slider.Slider;

/**
 * Swing implementation for Scilab Slider in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabSlider extends JSlider implements Slider {

	/**
	 * Constructor
	 */
	public SwingScilabSlider() {
		super();
		// Scilab slider always snap to ticks
		setSnapToTicks(true);
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
}
