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

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public interface HelpBrowser extends Dockable {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleHelpBrowser getAsSimpleHelpBrowser();

	/**
	 * Display the Help Browser
	 */
	void display();
	
	/**
	 * Display help page for a specified keyword
	 * @param keyword the keyword
	 */
	void searchKeywork(String keyword);
	
	/**
	 * Display the result of a full-text search for a specified word
	 * @param keyword the keyword
	 */
	void fullTextSearch(String keyword);
	
	/**
	 * Close the HelpBrowser
	 */
	void close();
}
