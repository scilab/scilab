
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.listbox;

import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;

/**
 * Bridge for Scilab ListBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabListBoxBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabListBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab ListBox
	 * @return the created ListBox
	 */
	public static ListBox createListBox() {
//		 FIXME : UI CONTROL must be developped last !!!!!
//		return new SwingScilabListBox();
		return null;
	}
}
