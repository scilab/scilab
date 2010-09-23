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

import java.util.Map;
import java.util.WeakHashMap;

import javax.swing.undo.AbstractUndoableEdit;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;

/**
 * Class to make undoable edits in EditVar
 * @author Calixte DENIZET
 */
public class CellsUndoableEdit extends AbstractUndoableEdit {

    private static Map<String, String> previousCommand = new WeakHashMap();

    private String oldCommand;
    private String newCommand;
    private SwingEditvarTableModel model;

    /**
     * Constructor
     * @param model the model where the edit occured
     */
    public CellsUndoableEdit(SwingEditvarTableModel model) {
        this.model = model;
        if (!previousCommand.containsKey(model.getVarName())) {
            previousCommand.put(model.getVarName(), model.getUndoManager().getInitCommand());
        }
        this.oldCommand = previousCommand.get(model.getVarName());
        this.newCommand = model.getCommandFromMatrix();
        previousCommand.put(model.getVarName(), newCommand);
    }

    /**
     * {@inheritDoc}
     */
    public void undo() {
        super.undo();
        model.updateCommand(oldCommand);
    }

    /**
     * {@inheritDoc}
     */
    public void redo() {
        super.redo();
        model.updateCommand(newCommand);
    }

    /**
     * @param var the variable to remove
     */
    public static void removeVar(String var) {
        previousCommand.remove(var);
    }

    /**
     * Clear the previous commands
     */
    public static void clear() {
        previousCommand.clear();
    }
}
