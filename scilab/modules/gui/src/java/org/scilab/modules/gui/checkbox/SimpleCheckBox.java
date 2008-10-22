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

package org.scilab.modules.gui.checkbox;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBoxBridge the associated object to Scilab GUIs Checkbox
 * @author Vincent COUVERT
 */
public interface SimpleCheckBox extends Widget {
	/**
	 * Set if the CheckBox is checked or not
	 * @param status true to set the CheckBox checked
	 */
	void setChecked(boolean status);
	
	/**
	 * Get the status of the CheckBox
	 * @return true if the CheckBox is checked
	 */
	boolean isChecked();
}
