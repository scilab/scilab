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

package org.scilab.modules.gui.bridge.radiogroup;

import javax.swing.ButtonGroup;

import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.radiogroup.RadioGroup;

/**
 * Swing implementation for Scilab RadioGroup in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabRadioGroup extends ButtonGroup implements RadioGroup {
	
	private static final long serialVersionUID = 5155598847554099496L;

	/**
	 * Constructor
	 */
	public SwingScilabRadioGroup() {
		super();
	}
	
	/**
	 * Add a RadioButton to a RadioGroup
	 * @param button the button to add
	 */
	public void add(RadioButton button) {
		super.add((SwingScilabRadioButton) button);
	}
	
	/**
	 * Remove a RadioButton from a RadioGroup.
	 * @param button the button to remove
	 */
	public void remove(RadioButton button) {
		super.remove((SwingScilabRadioButton) button);
	}
	
}
