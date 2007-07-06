
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.checkbox;

import javax.swing.JCheckBox;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.checkbox.CheckBox;

/**
 * Swing implementation for Scilab CheckBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabCheckBox extends JCheckBox implements CheckBox {

	/**
	 * Constructor
	 */
	public SwingScilabCheckBox() {
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
