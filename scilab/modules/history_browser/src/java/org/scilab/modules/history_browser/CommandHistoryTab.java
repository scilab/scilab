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
