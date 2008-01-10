
/* Copyright INRIA 2007 */

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
}
