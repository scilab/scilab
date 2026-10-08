/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;

/** Run with the debugger, or continue, up to the line of the caret. */
public final class DebugRunToCursorAction extends DefaultAction {

    private static final long serialVersionUID = 2954417756083621907L;

    public DebugRunToCursorAction(String name, SciNotes editor) {
        super(name, editor);
    }

    @Override
    public void doAction() {
        ScilabEditorPane pane = getEditor().getTextPane();
        if (pane instanceof MonacoEditorPane) {
            ((MonacoEditorPane) pane).runToCursor();
        }
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new DebugRunToCursorAction(label, editor), key);
    }
}
