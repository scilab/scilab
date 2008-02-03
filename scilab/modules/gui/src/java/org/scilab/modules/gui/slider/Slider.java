
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.slider;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for Slider associated to objects in Scilab GUIs
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
	 * Set the slider orientation to vertical
	 */
	void setVertical();

	/**
	 * Set the slider orientation to horizontal
	 */
	void setHorizontal();
}
