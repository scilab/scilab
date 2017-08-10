/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.ui_data.variableeditor.undo;

import javax.swing.undo.UndoableEdit;
import javax.swing.undo.UndoManager;

import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * Class CellsUndoManager
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class CellsUndoManager extends UndoManager {

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     */
    public CellsUndoManager(SwingScilabVariableEditor editor) {
        super();
        this.editor = editor;
        setLimit(-1);
    }

    /**
     * {@inheritDoc}
     */
    public void undo() {
        if (canUndo()) {
            super.undo();
            editor.enableRedoButton(true);
        }
        editor.enableUndoButton(canUndo());
    }

    /**
     * {@inheritDoc}
     */
    public void redo() {
        if (canRedo()) {
            super.redo();
            editor.enableUndoButton(true);
        }
        editor.enableRedoButton(canRedo());
    }

    /**
     * {@inheritDoc}
     */
    public boolean addEdit(UndoableEdit e) {
        if (e != null) {
            boolean b = super.addEdit(e);
            if (b) {
                editor.enableUndoButton(true);
            }
            return b;
        }

        return false;
    }
}
