
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.frame.Frame;

/**
 * Interface for tabs in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface Tab extends Container {

	/**
	 * Gets a much more dummy Objects.
	 * @return the component.
	 */
	SimpleTab getAsSimpleTab();
	
	/**
	 * Gets the Name of a tab
	 * @return the Name of the tab
	 */
	String getName();

	/**
	 * Sets the Name of a tab
	 * @param newTabName the Name we want to set to the tab
	 */
	void setName(String newTabName);

	// Watch out not to override or add some inherited profiles...
}
