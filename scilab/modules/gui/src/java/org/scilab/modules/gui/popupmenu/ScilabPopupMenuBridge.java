
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.popupmenu;

import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;

/**
 * Bridge for Scilab PopupMenus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabPopupMenuBridge {

	/**
	 * Constructor
	 */
	protected ScilabPopupMenuBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab PopupMenu
	 * @return the created PopupMenu
	 */
	public static PopupMenu createPopupMenu() {
		return new SwingScilabPopupMenu();
	}
	
}
