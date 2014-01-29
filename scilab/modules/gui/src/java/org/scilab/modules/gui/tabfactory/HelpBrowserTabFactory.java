/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
