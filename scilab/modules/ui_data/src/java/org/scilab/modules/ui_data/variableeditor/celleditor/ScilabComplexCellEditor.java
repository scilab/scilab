package org.scilab.modules.ui_data.variableeditor.celleditor;

import java.awt.Component;

import javax.swing.JTable;

public class ScilabComplexCellEditor  extends ScilabGenericCellEditor {
 
    private static final long serialVersionUID = 3866044279955696250L;
    
    /**
     * {@inheritDoc}
     */
    @Override
    public Component getTableCellEditorComponent(JTable table, Object value,
            boolean isSelected, int row, int col) {
        this.row = row + 1; // +1 because scilab index start at 1
        this.col = col + 1;

        Object newValue = value;
        if (value != null && value instanceof Double[]) {
        	Double[] oldValue = (Double[]) value; 
        	if (oldValue[1] != 0.0) {
        		newValue = oldValue[0] +" + " + oldValue[1] + " * %i";
        	} else {
        		newValue = oldValue[0];
        	}
        	
        	
        }

        if (value == "") {
            newValue = "0.0";
        }

        return super.getTableCellEditorComponent(table, newValue, isSelected, row, col);
    }
}