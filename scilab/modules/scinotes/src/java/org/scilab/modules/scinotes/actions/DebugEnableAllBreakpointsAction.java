/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;

/** Enable every breakpoint, in every file. */
public final class DebugEnableAllBreakpointsAction extends DefaultAction {

    private static final long serialVersionUID = 6203518847390142657L;

    public DebugEnableAllBreakpointsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    @Override
    public void doAction() {
        MonacoEditorPane.setAllBreakpointsEnabled(true);
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new DebugEnableAllBreakpointsAction(label, editor), key);
    }
}
