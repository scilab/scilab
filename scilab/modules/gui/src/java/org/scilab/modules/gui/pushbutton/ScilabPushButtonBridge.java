
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
	public static PushButton createPushButton() {
		return new SwingScilabPushButton();
	}

}

