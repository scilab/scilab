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

import java.awt.BorderLayout;

import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

/**
 * Class for the RowHeader
 * @author Calixte DENIZET
 */
public class RowHeader extends JPanel implements TableModelListener {

    private JTable rowTable;
    private DefaultTableModel model;

    /**
     * Constructor
     * @param table the table
     */
    public RowHeader(JTable table) {
        super(new BorderLayout());
        table.getModel().addTableModelListener(this);
        model = new DefaultTableModel(0, 1);
        rowTable = new JTable(model);
        rowTable.setModel(model);
        rowTable.setRowHeight(table.getRowHeight() + table.getRowMargin() - table.getIntercellSpacing().height);
        rowTable.getColumnModel().getColumn(0).setMinWidth(50);
        rowTable.getColumnModel().getColumn(0).setMaxWidth(100);
        rowTable.getColumnModel().getColumn(0).setPreferredWidth(50);

        rowTable.setDefaultEditor(Object.class, new RowHeaderCellEditor(table));
        rowTable.setDefaultRenderer(Object.class, new RowHeaderRenderer());
        rowTable.doLayout();
        add(rowTable);
        doLayout();
        setBackground(table.getSelectionBackground());
    }

    /**
     * {@inheritDoc}
     */
    public void tableChanged(TableModelEvent e) {
        DefaultTableModel dtm = (DefaultTableModel) e.getSource();
        int N = dtm.getRowCount() - model.getRowCount();
        for (int i = 0; i < N; i++) {
            model.addRow(new Object[1]);
        }
    }
}
