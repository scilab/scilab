/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/** Stop the current debug session and prepare a fresh attachment. */
public final class DebugDisconnectAction extends DebugCommandAction {

    private static final long serialVersionUID = 6171215679077433334L;

    public DebugDisconnectAction(String name, SciNotes editor) {
        super(name, editor, "disconnect");
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createDebugMenu(label, editor, key, new DebugDisconnectAction(label, editor));
    }

    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createDebugButton(tooltip, icon, editor, new DebugDisconnectAction(tooltip, editor));
    }
}
