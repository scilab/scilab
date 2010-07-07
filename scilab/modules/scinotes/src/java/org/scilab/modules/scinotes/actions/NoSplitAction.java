/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
 * No Split
 * @author Calixte DENIZET
 */
public final class NoSplitAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated editor
     */
    public NoSplitAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Create the MenuItem
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new NoSplitAction(label, editor), key);
    }

    /**
     * Action !!
     * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
     */
    public void doAction() {
        getEditor().removeSplit();
    }
}
