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
