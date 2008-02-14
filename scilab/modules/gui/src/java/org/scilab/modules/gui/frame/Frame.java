
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.events.callback.CallBack;

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
	 * Set the Background color of the Frame.
	 * @param color the Color
	 */
	void setBackground(Color color);
	
	/**
	 * Get the Background color of the Frame.
	 * @return the Color
	 */
	Color getBackground();
	
	/**
	 * Set the Foreground color of the Frame.
	 * @param color the Color
	 */
	void setForeground(Color color);
	
	/**
	 * Get the Foreground color of the Frame.
	 * @return the Color
	 */
	Color getForeground();

	/**
	 * Set the font of the Frame.
	 * @param font the font
	 */
	void setFont(Font font);
	
	/**
	 * Get the font of the Frame.
	 * @return the font
	 */
	Font getFont();
	
	/**
	 * Set the text of the Frame
	 * @param text the text
	 */
	void setText(String text);
	
	/**
	 * Get the text of the Frame
	 * @return the text
	 */
	String getText();

	/**
	 * Set the status of the Frame
	 * @param status true if the Widget is enabled
	 */
	void setEnabled(boolean status);

	/**
	 * Set the callback of the Frame
	 * @param callback the callback to set
	 */
	void setCallback(CallBack callback);
	
	/**
	 * Set the Relief of the Frame
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	void setRelief(String reliefType);
	
	/**
	 * Delete the Frame
	 */
	void destroy();
}
