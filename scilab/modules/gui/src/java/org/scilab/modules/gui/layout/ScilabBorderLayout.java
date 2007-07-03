
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.layout;

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabBorderLayout implements BorderLayout {
	
	/**
	 * Constructor
	 */
	protected ScilabBorderLayout() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/**
	 * Creates a Scilab BorderLayout object
	 * @return the created BorderLayout
	 */
	public static BorderLayout createBorderLayout() {
		return ScilabBridge.createBorderLayout();
	}
}
