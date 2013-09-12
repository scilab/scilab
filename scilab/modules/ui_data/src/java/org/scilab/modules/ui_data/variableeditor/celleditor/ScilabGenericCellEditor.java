/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
