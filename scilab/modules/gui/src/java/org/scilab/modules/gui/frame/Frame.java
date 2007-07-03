
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import java.awt.Color;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.layout.LayoutManager;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.Tab;

/**
 * Interface for Scilab frames in GUIs
 * @author Vincent COUVERT
 */
public interface Frame extends Container {

	/**
	 * We want to be able to add directly a Canvas in a Frame.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	int addMember(Canvas member);
	
	/**
	 * We want to be able to add a PushButton in a Frame.
	 * @param member the member to add
	 * @return the position of the PushButton in the member list.
	 */
	int addMember(PushButton member);
	
	/**
	 * We want to be able to add a PushButton in a Frame with a BorderLayout.
	 * @param member the PushButton to add
	 * @param borderLayout the BorderLayout to use
	 * @return the position of the PushButton in the member list.
	 */
	int addMember(PushButton member, String borderLayout);
	
	/**
	 * We want to be able to add a PushButton in a Frame with a FlowLayout.
	 * @param member the PushButton to add
	 * @param flowLayoutPosition the FlowLayout position to use
	 * @return the position of the PushButton in the member list.
	 */
	int addMember(PushButton member, int flowLayoutPosition);
	
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
	int addMember(Frame member, String borderLayout);
	
	/**
	 * We want to be able to add a Frame in a Frame with a FlowLayout.
	 * @param member the Frame to add
	 * @param flowLayoutPosition the FlowLayout position to use
	 * @return the position of the Frame in the member list.
	 */
	int addMember(Frame member, int flowLayoutPosition);
	
	// TODO : Check if this should be possible
	/**
	 * We want to be able to add a Tab in a Frame.
	 * @param member the member to add
	 * @return the position of the Tab in the member list.
	 */
	//int addMember(Tab member);

	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	void setBackground(Color color);
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	void setBorder(LineBorder lineBorder);
	
	/**
	 * To set the Layout of the element.
	 * @param layout the layout
	 */
	void setLayout(LayoutManager layout);
	
}
