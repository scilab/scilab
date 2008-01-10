
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.slider;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for RadioGroup associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Slider extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleSlider getAsSimpleSlider();

	/**
	 * Returns the slider's current value
	 * @return the current value of the slider
	 */
	//int getValue();
	
	/**
	 * Sets the slider's current value to n. This method forwards the new value to the model.
	 * If the new value is different from the previous value, all change listeners are notified.
	 * @param n - the new value
	 */
	//void setValue(int n);
	
	/**
     * This method sets the major tick spacing.
     * @param n - new value for the major tick spacing.
     */
	//void setMajorTickSpacing(int n);
	
	/**
	 * Sets the slider's maximum value to maximum.
	 * @param value the value to set
	 */
	//void setMaximum(int value);
	
	/**
	 * Sets the slider's minimum value to minimum.
	 * @param value the value to set
	 */
	//void setMinimum(int value);
	
	/**
     * This method sets the minior tick spacing.
     * @param n - new value for the minior tick spacing.
     */
	//void setMinorTickSpacing(int n);
}
