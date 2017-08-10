/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Display about box for the text editor
 * @author Calixte DENIZET
 */
public final class CCloseTabInNewWindowAction extends DefaultAction {

    private static final long serialVersionUID = 5238253626333724452L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated editor
     */
    public CCloseTabInNewWindowAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CCloseTabInNewWindowAction(label, editor), key);
    }

    /**
     * Action
     */
    public void doAction() {
        SciNotes.cloneAndCloseCurrentTab(getEditor(), true);
    }
}
