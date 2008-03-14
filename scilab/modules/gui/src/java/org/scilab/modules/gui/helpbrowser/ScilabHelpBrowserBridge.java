/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.helpbrowser;

import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;

/**
 * Bridge for Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public class ScilabHelpBrowserBridge {

	/**
	 * Constructor
	 */
	protected ScilabHelpBrowserBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Creates a Scilab HelpBrowser
	 * @param helps help chapters and directories
	 * @return the created HelpBrowser
	 */
	public static SimpleHelpBrowser createHelpBrowser(String[] helps) {
		return new SwingScilabHelpBrowser(helps);
	}

	/**
	 * Display the Help Browser
	 * @param browser the Help Browser
	 */
	public static void display(HelpBrowser browser) {
		browser.display();
	}
	
	/**
	 * Display the matching items for a specified keyword
	 * @param browser the Help Browser
	 * @param keyword the keyword
	 * @return true if the keyword exists
	 */
	public static boolean searchKeywork(HelpBrowser browser, String keyword) {
		return browser.getAsSimpleHelpBrowser().searchKeywork(keyword);
	}

	/**
	 * Close the HelpBrowser
	 * @param browser the Help Browser
	 */
	public static void close(HelpBrowser browser) {
		browser.getAsSimpleHelpBrowser().close();
	}

}
