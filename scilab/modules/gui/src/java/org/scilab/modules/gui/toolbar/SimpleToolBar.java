
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Interface for SimpleToolBar the associated object to Scilab GUIs ToolBar
 * @author Marouane BEN JELLOUL
 */
public interface SimpleToolBar {
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add.
	 */
	void add(PushButton pushButton);

	/**
	 * Add a Separator to a toolbar
	 */
	void addSeparator();

	/**
	 * Set the element id for this toolbar
	 * @param id the id of the corresponding toolbar object
	 */
	void setElementId(int id);
	
	/**
	 * Get the element id for this toolbar
	 * @return id the id of the corresponding toolbar object
	 */
	int getElementId();
	
}
