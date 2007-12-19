
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.widget;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for widget associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface Widget extends Dockable {

	/**
	 * Set the Background color of the widget.
	 * @param color the Color
	 */
	void setBackground(Color color);
	
	/**
	 * Get the Background color of the widget.
	 * @return the Color
	 */
	Color getBackground();
	
	/**
	 * Set the Foreground color of the widget.
	 * @param color the Color
	 */
	void setForeground(Color color);
	
	/**
	 * Get the Foreground color of the widget.
	 * @return the Color
	 */
	Color getForeground();

	/**
	 * Set the font of the widget.
	 * @param font the font
	 */
	void setFont(Font font);
	
	/**
	 * Get the font of the widget.
	 * @return the font
	 */
	Font getFont();

}
