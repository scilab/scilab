/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.tabfactory;

import java.util.List;
import java.util.ListIterator;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
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

        ClosingOperationsManager.registerClosingOperation((SwingScilabTab) nav,
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
                    public String askForClosing(final List<SwingScilabTab> list) {
                        return null;
                    }

                    @Override
                    public void updateDependencies(List<SwingScilabTab> list,
                            ListIterator<SwingScilabTab> it) {
                    }
                });

        WindowsConfigurationManager.registerEndedRestoration(
                (SwingScilabTab) nav,
                new WindowsConfigurationManager.EndedRestoration() {

                    @Override
                    public void finish() {
                        nav.changeToolBar();
                    }
                });

        ClosingOperationsManager.addDependency((SwingScilabTab) editor,
                (SwingScilabTab) nav);

        return nav;
    }
}
