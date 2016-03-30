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
import org.scilab.modules.scinotes.SciNotesGUI;

/**
 * Class to create SciNotes instances
 *
 * @author Calixte DENIZET
 */
public class SciNotesTab {

    /**
     * @param uuid
     *            the uuid to restore
     * @return a new SciNotes instance
     */
    public static SciNotes getEditorInstance(String uuid) {
        SciNotes editorInstance;
        if (uuid != null) {
            editorInstance = new SciNotes(uuid);
        } else {
            editorInstance = new SciNotes();
            ScilabTabFactory.getInstance().addToCache(editorInstance);
        }
        SciNotesGUI.init(editorInstance);

        final SciNotes ed = editorInstance;
        ClosingOperationsManager.registerClosingOperation(
            (SwingScilabDockablePanel) editorInstance,
        new ClosingOperationsManager.ClosingOperation() {

            @Override
            public int canClose() {
                return ed.canClose() ? 1 : 0;
            }

            @Override
            public void destroy() {
                ed.closeSciNotes();
            }

            @Override
            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return ed.askForClosing();
            }

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list,
                                           ListIterator<SwingScilabDockablePanel> it) {
            }
        });

        WindowsConfigurationManager.registerEndedRestoration(
            (SwingScilabDockablePanel) editorInstance,
        new WindowsConfigurationManager.EndedRestoration() {

            @Override
            public void finish() {
                ed.endedRestoration();
            }
        });

        ClosingOperationsManager
        .addDependencyWithRoot((SwingScilabDockablePanel) editorInstance);

        return editorInstance;
    }
}
