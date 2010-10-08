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

package org.scilab.modules.ui_data.variableeditor.undo;

import javax.swing.undo.UndoableEdit;
import javax.swing.undo.UndoManager;

import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * Class CellsUndoManager
 * @author Calixte DENIZET
 */
public class CellsUndoManager extends UndoManager {

    private String initCommand;
    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param initCommand the command user to create the edited matrix
     */
    public CellsUndoManager(SwingScilabVariableEditor editor, String initCommand) {
        super();
        this.editor = editor;
        this.initCommand = initCommand;
        setLimit(-1);
    }

    /**
     * @return the init command to have initial matrix
     */
    public String getInitCommand() {
        return initCommand;
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
