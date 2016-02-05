/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.variableeditor.renderers;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JTable;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;

/**
 * CellRenderer for boolean type
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabBooleanSparseRenderer extends ScilabBooleanRenderer {

    /**
     * Constructor
     */
    public ScilabBooleanSparseRenderer() {
        super();
    }

    protected static boolean isNothing(Object value) {
        return value == null || (value instanceof Boolean && !((Boolean) value).booleanValue());
    }

    /**
     * {@inheritDoc}
     */
    protected void setValue(Object value) {
        if (isNothing(value)) {
            super.setValue(null);
        } else {
            super.setValue(value);
        }
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
