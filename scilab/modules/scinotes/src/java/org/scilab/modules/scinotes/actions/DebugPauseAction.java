/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/** Pause a running debug execution. */
public final class DebugPauseAction extends DebugCommandAction {

    private static final long serialVersionUID = 1832387152949411796L;

    public DebugPauseAction(String name, SciNotes editor) {
        super(name, editor, "pause");
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createDebugMenu(label, editor, key, new DebugPauseAction(label, editor));
    }

    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createDebugButton(tooltip, icon, editor, new DebugPauseAction(tooltip, editor));
    }
}
