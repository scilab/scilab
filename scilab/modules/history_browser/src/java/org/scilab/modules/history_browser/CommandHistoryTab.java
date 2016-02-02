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

import java.util.List;
import java.util.ListIterator;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;

/**
 * Class to create Command History tab
 *
 * @author Calixte DENIZET
 */
public class CommandHistoryTab {

    /**
     * @param uuid
     *            the uuid to restore
     * @return a new Command History tab
     */
    public static SwingScilabDockablePanel getCommandHistoryInstance(String uuid) {
        final SwingScilabDockablePanel cmdh = CommandHistory.createCommandHistoryTab();
        ScilabTabFactory.getInstance().addToCache(cmdh);

        ClosingOperationsManager.registerClosingOperation(cmdh, new ClosingOperationsManager.ClosingOperation() {

            @Override
            public int canClose() {
                return 1;
            }

            @Override
            public void destroy() {
                CommandHistory.closeHistory();
            }

            @Override
            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return null;
            }

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list, ListIterator<SwingScilabDockablePanel> it) {
            }
        });

        ClosingOperationsManager.addDependencyWithRoot(cmdh);

        return cmdh;
    }
}
