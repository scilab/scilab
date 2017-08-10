/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.ui_data.datatable;

import javax.swing.ImageIcon;
import javax.swing.table.DefaultTableModel;

/**
 * Swing implementation of table model.
 * @param <Type> Type of data to be stored by the model
 */
public class SwingTableModel<Type> extends DefaultTableModel implements ScilabTable<Type> {

    private static final long serialVersionUID = -4255704246347716837L;

    private String[] columnNames;
    private Type[][] data;

    /**
     * Default construction setting columns title.
     * @param columnsNames : the columns title.
     */
    public SwingTableModel(String[] columnsNames) {
        this.columnNames = columnsNames;
    }

    public SwingTableModel(Type[][] data) {
        this.data = data;
    }

    /**
     * {@inheritDoc}
     */
    public int getColumnCount() {
        if (columnNames != null) {
            return columnNames.length;
        } else {
            return data[0].length;
        }
    }

    /**
     * {@inheritDoc}
     */
    public int getRowCount() {
        if (data == null) {
            return 0;
        }
        return data.length;
    }

    /**
     * {@inheritDoc}
     */
    public Type getValueAt(int row, int col) {
        if (data == null) {
            return null;
        }
        return data[row][col];
    }

    /**
     * {@inheritDoc}
     */
    public void setData(Type[][] data) {
        this.data = data;
    }

    /**
     * {@inheritDoc}
     */
    public String getColumnName(int col) {
        if (columnNames != null) {
            return columnNames[col];
        } else {
            return String.valueOf(col);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setColumnNames(String[] columnNames) {
        this.columnNames = columnNames;
    }

    /**
     * {@inheritDoc}
     */
    public Class getColumnClass(int c) {
        // Need to force return of ImageIcon when needed
        // to have real images displayed
        // and no PATH comming from "toString" method.
        if (c == 0) {
            return ImageIcon.class;
        }

        if (c == 2 || c == 3) {
            return Integer.class;
        }

        if (getValueAt(0, c) != null) {
            return getValueAt(0, c).getClass();
        }
        return Object.class;
    }

    @Override
    public boolean isCellEditable(int row, int column) {
        return false;
    }
}
