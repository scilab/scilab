/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
