/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/** Advance a stopped execution out of the current call. */
public final class DebugStepOutAction extends DebugCommandAction {

    private static final long serialVersionUID = 3992607119420754509L;

    public DebugStepOutAction(String name, SciNotes editor) {
        super(name, editor, "stepOut");
    }

    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createDebugMenu(label, editor, key, new DebugStepOutAction(label, editor));
    }

    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createDebugButton(tooltip, icon, editor, new DebugStepOutAction(tooltip, editor));
    }
}
