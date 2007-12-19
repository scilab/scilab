
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for SimplePushButton the associated object to Scilab GUIs PushButton
 * @author Marouane BEN JELLOUL
 */
public interface SimplePushButton {

	/**
	 * MUST IMPLEMENT UI CONTROL AT LEAST
	 * AND AN INCREMENTAL WAY !!!
	 */
	
	// TODO : Add the methods that are usefull for a PushButton
	
	/**
	 * Gets the size of an PushButton (width and height)
	 * @return the size of the PushButton
	 */
	Size getDims();

	/**
	 * Sets the size of an PushButton (width and height)
	 * @param newSize the size we want to set to the PushButton
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an PushButton (X-coordinate and Y-corrdinate)
	 * @return the position of the PushButton
	 */
	Position getPosition();

	/**
	 * Sets the position of an PushButton (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the PushButton
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an PushButton
	 * @return the visibility status of the PushButton (true if the PushButton is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an PushButton
	 * @param newVisibleState the visibility status we want to set for the PushButton
	 * 			(true if the PushButton is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an PushButton
	 */
	void draw();
	
	/**
	 * Sets the text of a PushButton
	 * @param text the text to set to the PushButton
	 */
	void setText(String text);

	/**
	 * Gets the text of a PushButton
	 * @return the text of the PushButton
	 */
	String getText();

	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	void setIcon(String filename);
	
	/**
	 * Add a callback to the pushbutton, this callback is a Scilab command
	 * @param command the Scilab command to execute when the pushbutton is activated
	 * @param commandType the type of the command that will be executed.
	 */
	void setCallback(String command, int commandType);

	/**
	 * Set if the pushbutton is enabled or not
	 * @param status true if the pushbutton is enabled
	 */
	void setEnabled(boolean status);

	/**
	 * To set the Background color of the simple pushbutton
	 * @param color the Color
	 */
	void setBackground(Color color);

	/**
	 * Get the Background color of the simple pushbutton
	 * @return the Color
	 */
	Color getBackground();

	/**
	 * To set the Foreground color of the simple pushbutton
	 * @param color the Color
	 */
	void setForeground(Color color);

	/**
	 * Get the Foreground color of the simple pushbutton
	 * @return the Color
	 */
	Color getForeground();

	/**
	 * Set the font of the simple pushbutton
	 * @param font the font
	 */
	void setFont(Font font);
	
	/**
	 * Get the font of the simple pushbutton
	 * @return the font
	 */
	Font getFont();

}
