
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.slider;

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
//		 FIXME : UI CONTROL must be developped last !!!!!
//		return new SwingScilabSlider();
		return null;
	}
}
