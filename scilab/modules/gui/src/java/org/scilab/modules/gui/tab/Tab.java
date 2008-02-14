
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.window.Window;

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
	 * We want to be able to add directly a label in a Tab.
	 * @param member the label to add
	 * @return the position of the label in the member list.
	 */
	int addMember(Label member);

	/**
	 * We want to be able to add directly a checkbox in a Tab.
	 * @param member the checkbox to add
	 * @return the position of the checkbox in the member list.
	 */
	int addMember(CheckBox member);

	/**
	 * We want to be able to add directly a RadioButton in a Tab.
	 * @param member the RadioButton to add
	 * @return the position of the RadioButton in the member list.
	 */
	int addMember(RadioButton member);

	/**
	 * We want to be able to add directly a Slider in a Tab.
	 * @param member the Slider to add
	 * @return the position of the Slider in the member list.
	 */
	int addMember(Slider member);

	/**
	 * We want to be able to add directly a ListBox in a Tab.
	 * @param member the ListBox to add
	 * @return the position of the ListBox in the member list.
	 */
	int addMember(ListBox member);

	/**
	 * We want to be able to add directly a PopupMenu in a Tab.
	 * @param member the PopupMenu to add
	 * @return the position of the PopupMenu in the member list.
	 */
	int addMember(PopupMenu member);
	
	/**
	 * We want to be able to remove directly a Canvas from a Tab.
	 * @param member canvas to remove 
	 */
	void removeMember(Canvas member);
	
	/**
	 * Destroy the tab.
	 */
	void close();

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
	
	/**
	 * Set the callback of the tab
	 * @param callback the CallBack to set
	 */
	void setCallback(CallBack callback);
	
	/**
	 * @return parent window of the tab object
	 */
	Window getParentWindow();

}
