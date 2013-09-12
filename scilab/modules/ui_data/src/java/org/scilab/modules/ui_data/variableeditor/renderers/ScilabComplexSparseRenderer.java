/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.renderers;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JTable;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;

/**
 * CellRenderer for sparse type
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabComplexSparseRenderer extends ScilabComplexRenderer {

    /**
     * Constructor
     */
    public ScilabComplexSparseRenderer() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    protected void setValue(Object value) {
        if (isNothing(value)) {
            value = "";
        }
        super.setValue(value);
    }

    protected static boolean isNothing(Object value) {
        return value == null || (value instanceof Double && ((Double) value).doubleValue() == 0) || (value instanceof Double[] && isNull((Double[]) value));
    }

    /**
     * {@inheritDoc}
     */
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if (isNothing(value) && !isSelected && ((SwingEditvarTableModel) table.getModel()).getScilabMatrixRowCount() > row && ((SwingEditvarTableModel) table.getModel()).getScilabMatrixColCount() > column) {
            setBackground(Color.LIGHT_GRAY);
        } else {
            setBackground(null);
        }
        return super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
    }
}
