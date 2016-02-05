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

package org.scilab.modules.core;

import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class ConsoleTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "Console";
    public static final String PACKAGE = "Console";
    public static final String CLASS = "org.scilab.modules.core.ConsoleTabFactory";

    private static ConsoleTabFactory instance;

    /**
     * Default constructor
     */
    public ConsoleTabFactory() { }

    public SwingScilabDockablePanel getTab(String uuid) {
        if (isAValidUUID(uuid)) {
            return (SwingScilabDockablePanel) ConsoleTab.getConsoleTab(uuid).getAsSimpleTab();
        }
        return null;
    }

    public String getPackage() {
        return PACKAGE;
    }

    public String getClassName() {
        return CLASS;
    }

    public String getApplication() {
        return APPLICATION;
    }

    public boolean isAValidUUID(String uuid) {
        return uuid.equals(SwingView.NULLUUID);
    }

    /**
     * @return an instance of this factory
     */
    public static ConsoleTabFactory getInstance() {
        if (instance == null) {
            instance = new ConsoleTabFactory();
        }

        return instance;
    }
}
