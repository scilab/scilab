
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.editbox;

import org.scilab.modules.gui.text.Text;

/**
 * Interface for editbox associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface EditBox extends Text {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleEditBox getAsSimpleEditBox();
}
