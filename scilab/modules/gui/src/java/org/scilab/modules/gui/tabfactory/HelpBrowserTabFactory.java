/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.gui.tabfactory;

import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class HelpBrowserTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "HelpBrowser";
    public static final String PACKAGE = "";
    public static final String CLASS = "org.scilab.modules.gui.tabfactory.HelpBrowserTabFactory";

    private static HelpBrowserTabFactory instance;

    /**
     * Default constructor
     */
    public HelpBrowserTabFactory() {
        if (instance == null) {
            instance = this;
        }
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabDockablePanel getTab(String uuid) {
        return HelpBrowserTab.getHelpBrowserInstance();
    }

    /**
     * {@inheritDoc}
     */
    public String getPackage() {
        return PACKAGE;
    }

    /**
     * {@inheritDoc}
     */
    public String getClassName() {
        return CLASS;
    }

    /**
     * {@inheritDoc}
     */
    public String getApplication() {
        return APPLICATION;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isAValidUUID(String uuid) {
        return ScilabHelpBrowser.HELPUUID.equals(uuid);
    }

    /**
     * @return an instance of this factory
     */
    public static HelpBrowserTabFactory getInstance() {
        new HelpBrowserTabFactory();

        return instance;
    }
}
