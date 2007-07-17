
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.dockable.Dockable;
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
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(Dockable member);
	
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


	/**
	 * WE ABSOLUTELY WANT TO AVOID
	 * USE OF ANY GUI IMPLEMENTATION OBJECTS !!!!
	 * 
	 * THE AIM IS NOT TO MIRROR THE IMPLEMENTATION !!!!
	 */
	 
//	
//	/**
//	 * To set the Border color and size of the element.
//	 * @param lineBorder the LineBorder
//	 */
//	void setBorder(LineBorder lineBorder);
//	
//	/**
//	 * To set the Layout of the element.
//	 * @param layout the layout
//	 */
//	void setLayout(LayoutManager layout);
//	
//	/**
//	 * To set the Background color of the element.
//	 * @param color the Color
//	 */
//	void setBackground(Color color);
//	
//	/**
//	 * To get the Background color of the element.
//	 * @return color the Color
//	 */
//	Color getBackground();
//	
//	/**
//	 * To set the Foreground color of the element.
//	 * @param color the Color
//	 */
//	void setForeground(Color color);
//	
//	/**
//	 * To get the Foreground color of the element.
//	 * @return color the Color
//	 */
//	Color getForeground();
//	
//	/**
//	 * To set the Font of the element.
//	 * @param font the Font
//	 */
//	void setFont(Font font);
//	
//	/**
//	 * To get the Font of the element.
//	 * @return font the Font
//	 */
//	Font getFont();
//	
//	/**
//	 * To set the Dimension of the element.
//	 * @param dimension the Dimension
//	 */
//	void setSize(Dimension dimension);
//	
//	/**
//	 * To get the Dimension of the element.
//	 * @return the Dimension
//	 */
//	Dimension getSize();
//	
//	/**
//	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
//	 * parameters in the coordinate space of this component's parent.
//	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
//	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
//	 */
//	void setLocation(int x, int y);
//	
//	/**
//	 * To get the x coordinate  of the element.
//	 * @return the x coordinate
//	 */
//	int getX();
//	
//	/**
//	 * To get the y coordinate  of the element.
//	 * @return the y coordinate
//	 */
//	int getY();
//	
//	/**
//	 * Sets the preferred size of the element.
//	 * @param dimension the Dimension
//	 */
//	void setPreferredSize(Dimension dimension);
//	
//	/**
//	 * If the preferredSize has been set to a non-null value just returns it. If the UI delegate's getPreferredSize
//	 *  method returns a non null value then return that; otherwise defer to the component's layout manager.
//	 * @return the preferredSize
//	 */
//	Dimension getPreferredSize();
//
//	/**
//	 * Sets the maximum size of the element.
//	 * @param dimension the Dimension
//	 */
//	void setMaximumSize(Dimension dimension);
//	
//	/**
//	 * If the maximum size has been set to a non-null value just returns it.
//	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
//	 * otherwise defer to the component's layout manager.
//	 * @return the maximum size
//	 */
//	Dimension getMaximumSize();
//	
//	/**
//	 * Sets the minimum size of the element.
//	 * @param dimension the Dimension
//	 */
//	void setMinimumSize(Dimension dimension);
//	
//	/**
//	 * If the minimum size has been set to a non-null value just returns it.
//	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
//	 * otherwise defer to the component's layout manager.
//	 * @return the minimum size
//	 */
//	Dimension getMinimumSize();
}
