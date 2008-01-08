
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.pushbutton.PushButton;

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

	/**
	 * We want to be able to add directly a Canvas in a Tab.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	int addMember(Canvas member);

	/**
	 * We want to be able to add directly a Console in a Tab.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	int addMember(Console member);
	
	/**
	 * We want to be able to add a Frame in a Tab.
	 * @param member the member to add
	 * @return the position of the Frame in the member list.
	 */
	int addMember(Frame member);
	
	/**
	 * We want to be able to add directly a pushbutton in a Tab.
	 * @param member the pushbutton to add
	 * @return the position of the pushbutton in the member list.
	 */
	int addMember(PushButton member);

	/**
	 * We want to be able to add directly a editbox in a Tab.
	 * @param member the editbox to add
	 * @return the position of the editbox in the member list.
	 */
	int addMember(EditBox member);

	/**
	 * Get the current status of the Tab in its parent
	 * @return true is the tab is the tab currently "on top" in its parent
	 */
	boolean isCurrentTab();
	
	/**
	 * Set the parent window id for this tab
	 * @param id the id of the parent window
	 */
	void setParentWindowId(int id);
	
	/**
	 * Get the parent window id for this tab
	 * @return the id of the parent window
	 */
	int getParentWindowId();
}
