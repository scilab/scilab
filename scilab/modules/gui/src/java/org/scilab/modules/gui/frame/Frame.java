
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.Tab;

/**
 * Interface for Scilab frames in GUIs
 * @author Vincent COUVERT
 */
public interface Frame extends Container {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleFrame getAsSimpleFrame();
	
	/**
	 * We want to be able to add directly a Canvas in a Frame.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	int addMember(Canvas member);

	/**
	 * We want to be able to add a CheckBox in a Frame.
	 * @param member the member to add
	 * @return the position of the CheckBox in the member list.
	 */
	int addMember(CheckBox member);

	/**
	 * We want to be able to add a EditBox in a Frame.
	 * @param member the member to add
	 * @return the position of the EditBox in the member list.
	 */
	int addMember(EditBox member);
	
	/**
	 * We want to be able to add a ListBox in a Frame.
	 * @param member the member to add
	 * @return the position of the ListBox in the member list.
	 */
	int addMember(ListBox member);
	
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
	int addMember(RadioButton member);

	/**
	 * We want to be able to add a Slider in a Frame.
	 * @param member the member to add
	 * @return the position of the slider in the member list.
	 */
	int addMember(Slider member);
	
	/**
	 * We want to be able to add a PushButton in a Frame with a BorderLayout.
	 * @param member the PushButton to add
	 * @param borderLayout the BorderLayout to use
	 * @return the position of the PushButton in the member list.
	 */
	/**
	 * JUST THINK SCILAB AND NOT JAVA !!! 
	 */
	//int addMember(PushButton member, String borderLayout);
	
	/**
	 * We want to be able to add a PushButton in a Frame with a FlowLayout.
	 * @param member the PushButton to add
	 * @param flowLayoutPosition the FlowLayout position to use
	 * @return the position of the PushButton in the member list.
	 */
	/**
	 * JUST THINK SCILAB AND NOT JAVA !!! 
	 */
	//int addMember(PushButton member, int flowLayoutPosition);
	
	/**
	 * We want to be able to add a Frame in a Frame.
	 * @param member the member to add
	 * @return the position of the Frame in the member list.
	 */
	int addMember(Frame member);
	
	/**
	 * We want to be able to add a Frame in a Frame with a BorderLayout.
	 * @param member the member to add
	 * @param borderLayout the BorderLayout to use
	 * @return the position of the Frame in the member list.
	 */
	/**
	 * JUST THINK SCILAB AND NOT JAVA !!! 
	 */
	//int addMember(Frame member, String borderLayout);
	
	/**
	 * We want to be able to add a Frame in a Frame with a FlowLayout.
	 * @param member the Frame to add
	 * @param flowLayoutPosition the FlowLayout position to use
	 * @return the position of the Frame in the member list.
	 */
	/**
	 * JUST THINK SCILAB AND NOT JAVA !!! 
	 */
	//int addMember(Frame member, int flowLayoutPosition);
	
	// TODO : Check if it should be possible to add a Tab to a frame and how it should behave
	/**
	 * We want to be able to add a Tab in a Frame.
	 * @param member the member to add
	 * @return the position of the Tab in the member list.
	 */
	int addMember(Tab member);
}
