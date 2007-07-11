
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.checkbox;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBox associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface CheckBox extends Widget {
	
	/**
	 * Sets the text of a CheckBox
	 * @param text the text to set to the CheckBox
	 */
	void setText(String text);

	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	void setBackground(Color color);
	
	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	Color getBackground();
	
	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	void setForeground(Color color);
	
	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	Color getForeground();
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	void setBorder(LineBorder lineBorder);
	
	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	void setFont(Font font);
	
	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	Font getFont();
	
	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	void setSize(Dimension dimension);
	
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	Dimension getSize();
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	void setLocation(int x, int y);
	
	/**
	 * To get the x coordinate  of the element.
	 * @return the x coordinate
	 */
	int getX();
	
	/**
	 * To get the y coordinate  of the element.
	 * @return the y coordinate
	 */
	int getY();
}
