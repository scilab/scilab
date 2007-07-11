
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.slider;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for RadioGroup associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface Slider extends Widget {

	/**
	 * Returns the slider's current value
	 * @return the current value of the slider
	 */
	int getValue();
	
	/**
	 * Sets the slider's current value to n. This method forwards the new value to the model.
	 * If the new value is different from the previous value, all change listeners are notified.
	 * @param n - the new value
	 */
	void setValue(int n);
	
	/**
     * This method sets the major tick spacing.
     * @param n - new value for the major tick spacing.
     */
	void setMajorTickSpacing(int n);
	
	/**
	 * Sets the slider's maximum value to maximum.
	 * @param value the value to set
	 */
	void setMaximum(int value);
	
	/**
	 * Sets the slider's minimum value to minimum.
	 * @param value the value to set
	 */
	void setMinimum(int value);
	
	/**
     * This method sets the minior tick spacing.
     * @param n - new value for the minior tick spacing.
     */
	void setMinorTickSpacing(int n);
	
	/**
	 * Tells if tick marks are to be painted.
	 * @return true if tick marks are painted, else false 
	 */
	boolean getPaintTicks();
	
	/**
	 * Determines whether tick marks are painted on the slider. By default, this property is false.
	 * @param b - whether or not tick marks should be painted
	 */
	void setPaintTicks(boolean b);
	
	/**
	 * Tells if labels are to be painted.
	 * @return true if labels are painted, else false
	 */
	boolean getPaintLabels();
	
	/**
	 * Determines whether tick marks are painted on the slider. By default, this property is false.
	 * By default, this property is false.
	 * @param b - whether or not to paint labels
	 */
	void setPaintLabels(boolean b);
	
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
