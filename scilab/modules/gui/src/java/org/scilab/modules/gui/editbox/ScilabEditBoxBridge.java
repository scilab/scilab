
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.editbox;

import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;

/**
 * Bridge for Scilab EditBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabEditBoxBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabEditBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab EditBox
	 * @return the created EditBox
	 */
	public static EditBox createEditBox() {
		return new SwingScilabEditBox();
	}
}
