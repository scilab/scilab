/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;

/** Disable every breakpoint, in every file, without removing them. */
public final class DebugDisableAllBreakpointsAction extends DefaultAction {

    private static final long serialVersionUID = 2915570463318204786L;

    public DebugDisableAllBreakpointsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    @Override
    public void doAction() {
        MonacoEditorPane.setAllBreakpointsEnabled(false);
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new DebugDisableAllBreakpointsAction(label, editor), key);
    }
}
