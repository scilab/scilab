
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.label;

import org.scilab.modules.gui.text.Text;

/**
 * Interface for Label associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public interface Label extends Text {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleLabel getAsSimpleLabel();

}
