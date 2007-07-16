
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;

/**
* Bridge for Scilab PushButtons in GUIs
* @author Marouane BEN JELLOUL
*/
public class ScilabPushButtonBridge {

	/**
	 * Constructor
	 */
	protected ScilabPushButtonBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new Scilab PushButton
	 * @return the created PushButton
	 */
	public static SimplePushButton createPushButton() {
		return new SwingScilabPushButton();
	}

	/**
	 * Sets the Text of a Scilab PushButton
	 * @param pushButton the PushButton we want to set the Text of
	 * @param newText the Text we want to set to the PushButton
	 */
	public static void setText(SimplePushButton pushButton, String newText) {
		pushButton.setText(newText);
	}
	
}

