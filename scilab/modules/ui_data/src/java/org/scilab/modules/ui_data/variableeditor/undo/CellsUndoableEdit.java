/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

import java.io.ObjectStreamField;
import java.io.Serializable;
import java.util.Vector;

import javax.swing.undo.AbstractUndoableEdit;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;

/**
 * Class to make undoable edits in EditVar
 * @author Calixte DENIZET
 */
public class CellsUndoableEdit extends AbstractUndoableEdit {
    private static final long serialVersionUID = 1L;

    // Explicit serialized form (see SwingTableModel for rationale) — oldValue/newValue
    // are declared Object to accept any cell value type, which javac's serial-lint
    // cannot statically prove Serializable. All fields are listed to preserve the
    // exact default serialization behavior.
    private static final ObjectStreamField[] serialPersistentFields = {
        new ObjectStreamField("oldValue", Object.class),
        new ObjectStreamField("newValue", Object.class),
        new ObjectStreamField("row", int.class),
        new ObjectStreamField("col", int.class),
        new ObjectStreamField("newRow", int.class),
        new ObjectStreamField("newCol", int.class),
        new ObjectStreamField("model", SwingEditvarTableModel.class),
    };

    private Object oldValue;
    private Object newValue;
    private int row;
    private int col;
    private int newRow;
    private int newCol;
    private SwingEditvarTableModel model;

    /**
     * Constructor
     * @param model the model where the edit occurred
     */
    public CellsUndoableEdit(SwingEditvarTableModel model, Object newValue, Object oldValue, int row, int col) {
        this.model = model;
        this.oldValue = oldValue;
        this.newValue = newValue;
        this.row = row;
        this.col = col;
        this.newRow = model.getScilabMatrixRowCount();
        this.newCol = model.getScilabMatrixColCount();

    }

    /**
     * {@inheritDoc}
     */
    public void undo() {
        super.undo();
        if (oldValue instanceof Vector) {
            model.changeData((Vector) oldValue, row, col);
        } else {
            model.setValueAtAndUpdate(true, false, oldValue, row, col);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void redo() {
        super.redo();
        if (newValue instanceof Vector) {
            model.changeData((Vector) newValue, newRow, newCol);
        } else {
            model.setValueAtAndUpdate(true, false, newValue, row, col);
        }
    }
}
