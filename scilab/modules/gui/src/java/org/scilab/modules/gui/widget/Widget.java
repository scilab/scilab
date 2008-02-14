
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.widget;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.events.callback.CallBack;

/**
 * Interface for widget associated to objects in Scilab GUIs
 * @author Vincent COUVERT
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
	
	/**
	 * Set the text of the widget
	 * @param text the text
	 */
	void setText(String text);
	
	/**
	 * Get the text of the widget
	 * @return the text
	 */
	String getText();

	/**
	 * Set the status of the widget
	 * @param status true if the Widget is enabled
	 */
	void setEnabled(boolean status);

	/**
	 * Set the callback of the widget
	 * @param callback the callback to set
	 */
	void setCallback(CallBack callback);

	/**
	 * Set the horizontal alignment for the widget text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	void setHorizontalAlignment(String alignment);

	/**
	 * Set the vertical alignment for the widget text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	void setVerticalAlignment(String alignment);

	/**
	 * Set the Relief of the Widget
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	void setRelief(String reliefType);
	
	/**
	 * Delete the Widget
	 */
	void destroy();
}
