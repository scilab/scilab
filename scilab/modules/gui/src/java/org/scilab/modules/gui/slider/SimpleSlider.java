/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
	void setUserValue(int value);

	/**
	 * Set the slider orientation to vertical
	 */
	void setVertical();

	/**
	 * Set the slider orientation to horizontal
	 */
	void setHorizontal();
}
