
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.textbox;

import javax.swing.JTextArea;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.textbox.TextBox;

/**
 * Swing implementation for Scilab TextBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTextBox extends JTextArea implements TextBox {

	/**
	 * Constructor
	 */
	public SwingScilabTextBox() {
		super();
		setEditable(false);
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
}
