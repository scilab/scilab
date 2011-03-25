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

package org.scilab.modules.ui_data.variableeditor.actions;

import javax.swing.KeyStroke;
import javax.swing.JTable;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * SupprAction class
 * @author Calixte DENIZET
 */
public final class SupprAction extends CallBack {

    private static final String KEY = "DELETE";
    private static final String SUPPR = "Suppr";

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public SupprAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(SUPPR, new SupprAction(editor, SUPPR));
        table.getInputMap().put(KeyStroke.getKeyStroke(KEY), SUPPR);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
            table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);
            for (int i = rows[rows.length - 1]; i >= rows[0]; i--) {
                for (int j = cols[cols.length - 1]; j >= cols[0]; j--) {
                    model.getCellEditor().removeExpression(i, j);
                    model.emptyValueAt(i, j);
                }
                model.removeRow(i, cols[0], cols[cols.length - 1]);
            }
            for (int j = cols[cols.length - 1]; j >= cols[0]; j--) {
                model.removeCol(table, j, rows[0], rows[rows.length - 1]);
            }
            model.updateMatrix();
        }
    }
}
