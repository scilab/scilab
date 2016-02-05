/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

    /**
     * Show the search field in Help Browser
     */
    void showSearchField();

    /**
     * Increase the font in the viewer
     */
    void increaseFont();

    /**
     * Decrease the font in the viewer
     */
    void decreaseFont();
}
