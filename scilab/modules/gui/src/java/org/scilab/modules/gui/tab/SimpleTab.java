
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.frame.Frame;

/**
 * Interface for tabs in Scilab GUIs
 * Describe all what the "widget" tab
 * must be able to do as a GUI Component
 * @author Bruno JOFRET
 */
public interface SimpleTab  {

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

	/**
	 * We want to be able to add directly a Frame in a Tab.
	 * @param member the member to add
	 * @return the position of the frame in the member list.
	 */
	int addMember(Frame member);

	/**
	 * We want to be able to add directly a Console in a Tab.
	 * @param member the member to add
	 * @return the position of the console in the member list.
	 */
	int addMember(Console member);

	/**
	 * We want to be able to add directly a Console in a Tab.
	 * @param member the member to add
	 * @return the position of the console in the member list.
	 */
	int addMember(Canvas member);
}
