/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO -  Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/**
 * CloseAllButThisAction Class
 * @author Allan CORNET
 *
 */
public final class CloseAllButThisAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -9052217229357472945L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CloseAllButThisAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        int nbTabCount = getEditor().getTabPane().getTabCount();
        boolean bContinue = true;
        if (nbTabCount > 1) {
            while ((getEditor().getTabPane().getTabCount() != 1) & (bContinue)) {
                int currentIndex = getEditor().getTabPane().getSelectedIndex();
                if (currentIndex != 0) {
                    bContinue = getEditor().closeTabAt(0);
                } else {
                    bContinue = getEditor().closeTabAt(1);
                }
            }
        }

        // Close the last opened file create a new file named "Untitled 1"
        if (getEditor().getTabPane().getTabCount() == 0) {
            getEditor().addEmptyTab();
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CloseAllButThisAction(label, editor), key);
    }
}
