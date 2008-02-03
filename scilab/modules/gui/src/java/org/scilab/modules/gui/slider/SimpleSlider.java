
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.slider;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SliderBridge the associated object to Scilab GUIs Slider
 * @author Vincent COUVERT
 */
public interface SimpleSlider extends Widget {
	
	/**
	 * Set the minor tick spacing for a Slider
	 * @param space the increment value
	 */
	void setMinorTickSpacing(int space);

	/**
	 * Set the major tick spacing for a Slider
	 * @param space the increment value
	 */
	void setMajorTickSpacing(int space);
	
	/**
	 * Set the minimum value of a Slider
	 * @param value the minimum value
	 */
	void setMinimumValue(int value);

	/**
	 * Set the maximum value of a Slider
	 * @param value the maximum value
	 */
	void setMaximumValue(int value);
	
	/**
	 * Get the current value of the Slider
	 * @return the current value of the Slider
	 */
	int getValue();
	
	/**
	 * Sets the current value of the Slider
	 * @param value the new value
	 */
	void setValue(int value);

	/**
	 * Set the slider orientation to vertical
	 */
	void setVertical();

	/**
	 * Set the slider orientation to horizontal
	 */
	void setHorizontal();
}
