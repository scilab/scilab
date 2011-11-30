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

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesGUI;

/**
 * Class to create SciNotes instances
 * @author Calixte DENIZET
 */
public class SciNotesTab {

    /**
     * @param uuid the uuid to restore
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
        ClosingOperationsManager.registerClosingOperation((SwingScilabTab) editorInstance, new ClosingOperationsManager.ClosingOperation() {

                public boolean canClose() {
                    return ed.canClose();
                }

                public void destroy() {
                    ed.closeSciNotes();
                }

                public String askForClosing(final List<SwingScilabTab> list) {
                    return ed.askForClosing();
                }
            });

        WindowsConfigurationManager.registerEndedRestoration((SwingScilabTab) editorInstance, new WindowsConfigurationManager.EndedRestoration() {

                public void finish() {
                    ed.endedRestoration();
                }
            });

        ClosingOperationsManager.addDependencyWithRoot((SwingScilabTab) editorInstance);

        return editorInstance;
    }
}
