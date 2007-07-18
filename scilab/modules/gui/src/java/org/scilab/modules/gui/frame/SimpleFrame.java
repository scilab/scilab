
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;


/**
 * Interface for SimpleFrame the associated object to Scilab GUIs Frame
 * @author Marouane BEN JELLOUL
 */
public interface SimpleFrame {

	/**
	 * DO NOT BE OVER OPTIMISTIC !!!
	 * TRY TO FOCUS ON MAIN STUFF FIRST !!!
	 */
	
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	//int addMember(SimpleDockable member);
	
	/**
	 * We want to be able to add directly a Canvas in a Frame.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	//int addMember(SimpleCanvas member);

	/**
	 * We want to be able to add a CheckBox in a Frame.
	 * @param member the member to add
	 * @return the position of the CheckBox in the member list.
	 */
	//int addMember(CheckBox member);

	/**
	 * We want to be able to add a EditBox in a Frame.
	 * @param member the member to add
	 * @return the position of the EditBox in the member list.
	 */
	//int addMember(EditBox member);
	
	/**
	 * We want to be able to add a ListBox in a Frame.
	 * @param member the member to add
	 * @return the position of the ListBox in the member list.
	 */
	//int addMember(ListBox member);
	
	/**
	 * We want to be able to add a PushButton in a Frame.
	 * @param member the member to add
	 * @return the position of the PushButton in the member list.
	 */
	//int addMember(SimplePushButton member);
	
	/**
	 * We want to be able to add a RadioButton in a Frame.
	 * @param member the member to add
	 * @return the position of the RadioButton in the member list.
	 */
	//int addMember(RadioButton member);

	/**
	 * We want to be able to add a Slider in a Frame.
	 * @param member the member to add
	 * @return the position of the slider in the member list.
	 */
	//int addMember(Slider member);
	
	/**
	 * We want to be able to add a Frame in a Frame.
	 * @param member the member to add
	 * @return the position of the Frame in the member list.
	 */
	//int addMember(SimpleFrame member);
	
	// TODO : Check if it should be possible to add a Tab to a frame and how it should behave
	/**
	 * We want to be able to add a Tab in a Frame.
	 * @param member the member to add
	 * @return the position of the Tab in the member list.
	 */
	//int addMember(SimpleTab member);
	
	/**
	 * We want to be able to add a TextBox in a Frame.
	 * @param member the member to add
	 * @return the position of the TextBox in the member list.
	 */
	//int addMember(SimpleTextBox member);
}
