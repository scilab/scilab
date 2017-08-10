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

import java.util.Vector;

import javax.swing.undo.AbstractUndoableEdit;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;

/**
 * Class to make undoable edits in EditVar
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class CellsUndoableEdit extends AbstractUndoableEdit {

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
