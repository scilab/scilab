
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.radiobutton;

import javax.swing.JRadioButton;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.radiobutton.RadioButton;

/**
 * Swing implementation for Scilab RadioButton in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabRadioButton extends JRadioButton implements RadioButton {

	/**
	 * Constructor
	 */
	public SwingScilabRadioButton() {
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
