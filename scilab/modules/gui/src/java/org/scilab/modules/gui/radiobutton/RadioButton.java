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

package org.scilab.modules.gui.radiobutton;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for RadioButton associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface RadioButton extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleRadioButton getAsSimpleRadioButton();

	/**
	 * Set if the RadioButton is checked or not
	 * @param status true to set the RadioButton checked
	 */
	void setChecked(boolean status);
	
	/**
	 * Get the status of the RadioButton
	 * @return true if the RadioButton is checked
	 */
	boolean isChecked();
}
