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

package org.scilab.modules.scinotes.tabfactory;

import java.util.List;
import java.util.ListIterator;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.NavigatorWindow;

/**
 * Class to create SciNotes instances
 *
 * @author Calixte DENIZET
 */
public class CodeNavigatorTab {

    /**
     * @param uuid
     *            the uuid to restore
     * @return a new SciNotes instance
     */
    public static NavigatorWindow getCodeNavigatorInstance(
        final SciNotes editor, String uuid) {
        final NavigatorWindow nav;
        if (uuid != null) {
            nav = new NavigatorWindow(editor, uuid);
        } else {
            nav = new NavigatorWindow(editor);
            ScilabTabFactory.getInstance().addToCache(nav);
        }

        ClosingOperationsManager.registerClosingOperation((SwingScilabDockablePanel) nav,
        new ClosingOperationsManager.ClosingOperation() {

            @Override
            public int canClose() {
                return 1;
            }

            @Override
            public void destroy() {
                nav.closeNavigator();
            }

            @Override
            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return null;
            }

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list,
                                           ListIterator<SwingScilabDockablePanel> it) {
            }
        });

        WindowsConfigurationManager.registerEndedRestoration(
            (SwingScilabDockablePanel) nav,
        new WindowsConfigurationManager.EndedRestoration() {

            @Override
            public void finish() {
                nav.changeToolBar();
            }
        });

        ClosingOperationsManager.addDependency((SwingScilabDockablePanel) editor,
                                               (SwingScilabDockablePanel) nav);

        return nav;
    }
}
