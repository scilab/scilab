
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.widget.Widget;

/**
* Interface for PushButton associated to objects in Scilab GUIs
* @author Marouane BEN JELLOUL
*/
public interface PushButton extends Widget {
	// TODO : Add the methods that are usefull for a PushButton
	
	/**
	 * Sets the text of a PushButton
	 * @param text the text to set to the PushButton
	 */
	void setText(String text);

	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	void setIcon(String filename);
	
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
	 * To enable or disable the PushButton.
	 * @param value true to enable or false to disable
	 */
	void setEnabled(boolean value);
	
	/**
	 * check if the PushButton is enable or disable.
	 * @return true if enable or false if disable
	 */
	boolean isEnabled();
	
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
	
	/**
	 * Sets the preferred size of the element.
	 * @param dimension the Dimension
	 */
	void setPreferredSize(Dimension dimension);
	
	/**
	 * If the preferredSize has been set to a non-null value just returns it. If the UI delegate's getPreferredSize
	 *  method returns a non null value then return that; otherwise defer to the component's layout manager.
	 * @return the preferredSize
	 */
	Dimension getPreferredSize();

	/**
	 * Sets the maximum size of the element.
	 * @param dimension the Dimension
	 */
	void setMaximumSize(Dimension dimension);
	
	/**
	 * If the maximum size has been set to a non-null value just returns it.
	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
	 * otherwise defer to the component's layout manager.
	 * @return the maximum size
	 */
	Dimension getMaximumSize();
	
	/**
	 * Sets the minimum size of the element.
	 * @param dimension the Dimension
	 */
	void setMinimumSize(Dimension dimension);
	
	/**
	 * If the minimum size has been set to a non-null value just returns it.
	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
	 * otherwise defer to the component's layout manager.
	 * @return the minimum size
	 */
	Dimension getMinimumSize();
	
}
