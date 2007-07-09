
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.slider;

import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;

/**
 * Bridge for Scilab Sliders in GUIs
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
	public static Slider createSlider() {
		return new SwingScilabSlider();
	}
}
