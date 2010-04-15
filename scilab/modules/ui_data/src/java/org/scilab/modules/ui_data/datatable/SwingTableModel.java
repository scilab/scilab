/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.datatable;

import javax.swing.ImageIcon;
import javax.swing.table.AbstractTableModel;

/**
 * Swing implementation of table model.
 * @param <Type>
 */
public class SwingTableModel<Type> extends AbstractTableModel implements ScilabTable<Type> {

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

    /**
     * {@inheritDoc}
     */
    public int getColumnCount() {
	return columnNames.length;
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
	return columnNames[col];
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
	return Object.class;
    }
}
