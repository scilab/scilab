/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Delete action
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class DeleteAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1818764947112443369L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public DeleteAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
        doc.mergeEditsBegin();
        if (!getEditor().getTextPane().removeColumnSelection()) {
            getEditor().getTextPane().getActionMap().get(DefaultEditorKit.deleteNextCharAction).actionPerformed(null);
        }
        doc.mergeEditsEnd();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new DeleteAction(label, editor), key);
    }
}
