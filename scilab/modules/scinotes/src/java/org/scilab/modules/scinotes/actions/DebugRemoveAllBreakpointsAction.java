/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;

/** Remove every breakpoint, in every file. */
public final class DebugRemoveAllBreakpointsAction extends DefaultAction {

    private static final long serialVersionUID = 7340962185523871094L;

    public DebugRemoveAllBreakpointsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    @Override
    public void doAction() {
        MonacoEditorPane.removeAllBreakpoints();
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new DebugRemoveAllBreakpointsAction(label, editor), key);
    }
}
