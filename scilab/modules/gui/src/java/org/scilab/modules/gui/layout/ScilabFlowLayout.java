
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.layout;

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabFlowLayout implements FlowLayout {
	
	/**
	 * Constructor
	 */
	protected ScilabFlowLayout() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/**
	 * Creates a Scilab FlowLayout object
	 * @return the created FlowLayout
	 */
	public static FlowLayout createFlowLayout() {
		return ScilabBridge.createFlowLayout();
	}
}
