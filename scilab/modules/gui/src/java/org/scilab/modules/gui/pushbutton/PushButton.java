
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import java.awt.Color;

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
	 * To enable or disable the PushButton.
	 * @param value true to enable or false to disable
	 */
	void setEnabled(boolean value);
}
