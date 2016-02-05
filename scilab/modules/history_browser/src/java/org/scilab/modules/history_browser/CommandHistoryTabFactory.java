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
