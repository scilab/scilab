/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
 * NextTabAction Class
 * @author Bruno JOFRET
 *
 */
public final class NextTabAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 3283491475309953312L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public NextTabAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        int index = getEditor().getTabPane().getSelectedIndex();
        int count = getEditor().getTabPane().getTabCount();
        getEditor().getTabPane().setSelectedIndex((index + 1) % count);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new NextTabAction(label, editor), key);
    }
}
