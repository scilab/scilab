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

package org.scilab.modules.ui_data.rowheader;

import java.awt.event.ActionEvent;
import java.awt.Component;

import javax.swing.AbstractAction;
import javax.swing.AbstractCellEditor;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;

/**
 * @author Calixte DENIZET
 */
public class RowHeaderCellEditor extends AbstractCellEditor implements TableCellEditor {

    private JButton button;
    private int row;

    /**
     * Constructor
     * @param table the table where to put the rowHeader
     */
    public RowHeaderCellEditor(final JTable table) {
        button = new JButton();
        button.setFocusPainted(false);
        button.setAction(new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    table.setRowSelectionInterval(row, row);
                    table.setColumnSelectionInterval(0, table.getColumnCount() - 1);
                    table.requestFocus();
                }
            });
    }

    /**
     * {@inheritDoc}
     */
    public Object getCellEditorValue() {
        return row;
    }

    /**
     * {@inheritDoc}
     */
    public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int col) {
        this.row = row;
        button.setText(Integer.toString(row + 1));

        return button;
    }
}
