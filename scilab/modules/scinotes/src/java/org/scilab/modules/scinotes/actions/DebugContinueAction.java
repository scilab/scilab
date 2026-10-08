/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;

/** Continue a stopped debug execution, or save the file and start one. */
public final class DebugContinueAction extends DebugCommandAction {

    private static final long serialVersionUID = 4779578451129698178L;

    public DebugContinueAction(String name, SciNotes editor) {
        super(name, editor, "continue");
    }

    @Override
    public void doAction() {
        ScilabEditorPane pane = getEditor().getTextPane();
        if (pane instanceof MonacoEditorPane
                && ((MonacoEditorPane) pane).continueDapExecution()) {
            return;
        }
        new SaveAndExecuteFileAction("", getEditor(), true).doAction();
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createDebugMenu(label, editor, key, new DebugContinueAction(label, editor));
    }

    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createDebugButton(tooltip, icon, editor, new DebugContinueAction(tooltip, editor));
    }
}
