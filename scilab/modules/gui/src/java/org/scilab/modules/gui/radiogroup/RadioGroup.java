/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.radiogroup;

import org.scilab.modules.gui.radiobutton.RadioButton;

/**
 * Interface for RadioGroup associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface RadioGroup {

	/**
	 * add a RadioButton in a RadioGroup.
	 * @param button the button to add
	 */
	void add(RadioButton button);
	
	/**
	 * remove a RadioButton from a RadioGroup.
	 * @param button the button to remove
	 */
	void remove(RadioButton button);
}
