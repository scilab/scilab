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
     * @param language Scilab current language
     * @return the created HelpBrowser
     */
    public static SimpleHelpBrowser createHelpBrowser(String[] helps, String language) {
        return new SwingScilabHelpBrowser(helps, language);
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
     */
    public static void searchKeywork(HelpBrowser browser, String keyword) {
        browser.getAsSimpleHelpBrowser().searchKeywork(keyword);
    }

    /**
     * Display the result of a full-text search for a specified word
     * @param browser the Help Browser
     * @param keyword the keyword
     */
    public static void fullTextSearch(HelpBrowser browser, String keyword) {
        browser.getAsSimpleHelpBrowser().fullTextSearch(keyword);
    }

    /**
     * Close the HelpBrowser
     * @param browser the Help Browser
     */
    public static void close(HelpBrowser browser) {
        browser.getAsSimpleHelpBrowser().close();
    }

}
