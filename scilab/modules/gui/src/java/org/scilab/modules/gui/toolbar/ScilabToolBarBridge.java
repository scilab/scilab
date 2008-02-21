/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Bridge for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public class ScilabToolBarBridge {

	/**
	 * Constructor
	 */
	protected ScilabToolBarBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab ToolBar
	 * @return the created ToolBar
	 */
	public static SimpleToolBar createToolBar() {
		return new SwingScilabToolBar();
	}
	
	/**
	 * Append a pushButton to a toolBar
	 * @param toolBar the ToolBar which we want to add the pushButton to
	 * @param pushButton the PushButton to add to the toolBar
	 */
	public static void add(ToolBar toolBar, PushButton pushButton) {
		toolBar.getAsSimpleToolBar().add(pushButton);
	}

	/**
	 * Add a Separator to a Toolbar
	 * @param toolbar the toolbar which we want to add the Separator to
	 */
	public static void addSeparator(ToolBar toolbar) {
		toolbar.getAsSimpleToolBar().addSeparator();
	}

	/**
	 * Set the visibility of the Toolbar
	 * @param toolbar the ToolBar
	 * @param newVisibleState true to set the toolbar visible
	 */
	public static void setVisible(ToolBar toolbar, boolean newVisibleState) {
		toolbar.getAsSimpleToolBar().setVisible(newVisibleState);
	}
	
	/**
	 * Get the visibility of the TooBar
	 * @param toolbar the ToolBar
	 * @return true if the ToolBar is Visible
	 */
	public static boolean isVisible(ToolBar toolbar) {
		return toolbar.getAsSimpleToolBar().isVisible();
	}



}
