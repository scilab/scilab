/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Class Previous tab action for SciNotes
 * @author Sylvestre Koumar
 *
 */
@SuppressWarnings(value = { "serial" })
public class PreviousTabAction extends DefaultAction {

    /**
     * Default constructor
     * @param name the name of the action
     * @param editor Editor
     */
    public PreviousTabAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        int index = getEditor().getTabPane().getSelectedIndex();
        int count = getEditor().getTabPane().getTabCount();
        getEditor().getTabPane().setSelectedIndex((count + index - 1) % count);
    }

    /**
     * Create the MenuItem for previous tab action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new PreviousTabAction(label, editor), key);
    }
}
