
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.editbox;

import javax.swing.JTextArea;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.editbox.EditBox;

/**
 * Swing implementation for Scilab EditBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabEditBox extends JTextArea implements EditBox {
	/**
	 * Constructor
	 */
	public SwingScilabEditBox() {
		super();
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
}
