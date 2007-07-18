
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;


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
	 * Gets the size of an Frame (width and height)
	 * @return the size of the Frame
	 */
	Size getDims();

	/**
	 * Sets the size of an Frame (width and height)
	 * @param newSize the size we want to set to the Frame
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an Frame (X-coordinate and Y-corrdinate)
	 * @return the position of the Frame
	 */
	Position getPosition();

	/**
	 * Sets the position of an Frame (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the Frame
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an Frame
	 * @return the visibility status of the Frame (true if the Frame is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an Frame
	 * @param newVisibleState the visibility status we want to set for the Frame
	 * 			(true if the Frame is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an Frame
	 */
	void draw();
	
	/**
	 * We want to be able to add directly a Canvas in a Frame.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	//int addMember(Canvas member);

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
	int addMember(PushButton member);
	
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
	int addMember(Frame member);
	
	// TODO : Check if it should be possible to add a Tab to a frame and how it should behave
	/**
	 * We want to be able to add a Tab in a Frame.
	 * @param member the member to add
	 * @return the position of the Tab in the member list.
	 */
	//int addMember(Tab member);
	
	/**
	 * We want to be able to add a TextBox in a Frame.
	 * @param member the member to add
	 * @return the position of the TextBox in the member list.
	 */
	//int addMember(TextBox member);
	
	
	
}
