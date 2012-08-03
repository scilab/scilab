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
import javax.swing.text.Element;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * GotoNextAnchorAction Class
 * @author Calixte DENIZET
 */
public class GotoNextAnchorAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public GotoNextAnchorAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        Element root = doc.getDefaultRootElement();
        int line = root.getElementIndex(sep.getCaretPosition());
        sep.scrollTextToLineNumber(doc.nextAnchorFrom(line) + 1, false);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new GotoNextAnchorAction(label, editor), key);
    }
}
