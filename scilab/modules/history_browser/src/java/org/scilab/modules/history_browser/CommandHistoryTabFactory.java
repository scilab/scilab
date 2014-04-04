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

package org.scilab.modules.history_browser;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class CommandHistoryTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "CommandHistory";
    public static final String PACKAGE = "";
    public static final String CLASS = "org.scilab.modules.history_browser.CommandHistoryTabFactory";

    private static CommandHistoryTabFactory instance;

    /**
     * Default constructor
     */
    public CommandHistoryTabFactory() {
        if (instance == null) {
            instance = this;
        }
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabDockablePanel getTab(String uuid) {
        return CommandHistoryTab.getCommandHistoryInstance(uuid);
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
        return CommandHistory.COMMANDHISTORYUUID.equals(uuid);
    }

    /**
     * @return an instance of this factory
     */
    public static CommandHistoryTabFactory getInstance() {
        new CommandHistoryTabFactory();

        return instance;
    }
}
