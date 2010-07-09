/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.celleditor;

import java.awt.Component;

import javax.swing.JTable;

public class ScilabBooleanCellEditor  extends ScilabGenericCellEditor {
 
    private static final long serialVersionUID = 3866054279955696250L;
    
    /**
     * {@inheritDoc}
     */
    @Override
    public Component getTableCellEditorComponent(JTable table, Object value,
            boolean isSelected, int row, int col) {
        this.row = row + 1; // +1 because scilab index start at 1
        this.col = col + 1;

        Object newValue = value;
        if (value != null && value instanceof Boolean) {
            if ((Boolean) value == false) {
                newValue = "%F" ;
            } else {
                newValue = "%T" ;
            }
        }

        if (value == "") {
            newValue = "%F";
        }

        return super.getTableCellEditorComponent(table, newValue, isSelected, row, col);
    }

}
