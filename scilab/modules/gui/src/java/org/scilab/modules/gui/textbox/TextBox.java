
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.text.Text;

/**
 * Interface for textbox associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface TextBox extends Text {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleTextBox getAsSimpleTextBox();

}
