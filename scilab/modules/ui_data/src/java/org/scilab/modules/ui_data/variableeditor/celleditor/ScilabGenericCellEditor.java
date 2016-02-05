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

package org.scilab.modules.ui_data.variableeditor.celleditor;

import java.awt.Component;

import javax.swing.BorderFactory;
import javax.swing.DefaultCellEditor;
import javax.swing.JFormattedTextField;
import javax.swing.JTable;

/**
 * @author Calixte DENIZET
 */
public class ScilabGenericCellEditor extends DefaultCellEditor {

    private static final long serialVersionUID = -5713675180587317165L;

    /**
     * Constructor
     */
    public ScilabGenericCellEditor() {
        super(new JFormattedTextField());
        ((JFormattedTextField) getComponent()).setBorder(BorderFactory.createEmptyBorder());
    }

    /**
     * @param value the value
     * @return the Scilab's representation of the value
     */
    public String getDataAsScilabString(Object value) {
        return "";
    }

    /**
     * {@inheritDoc}
     */
    public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int col) {
        return super.getTableCellEditorComponent(table, getDataAsScilabString(value), isSelected, row, col);
    }
}
