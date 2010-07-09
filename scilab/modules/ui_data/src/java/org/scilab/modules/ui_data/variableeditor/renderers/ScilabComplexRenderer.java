package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

public class ScilabComplexRenderer extends DefaultTableCellRenderer {
	
	public  ScilabComplexRenderer() {
		super();
	}

	@Override
	protected void setValue(Object value) {

		if (value == null){
			super.setValue("");
		} else if (value instanceof Double[]) {
        	Double[] oldValue = (Double[]) value; 
        	
        	if (oldValue[1] != 0.0 && oldValue[0] == 0.0) {
        		
        		value = oldValue[1] + "i";
        		
        	} else if (oldValue[1] == 0.0 && oldValue[0] != 0.0) {
        		
        		value = "" + oldValue[0];
        		
        	} else if (oldValue[1] != 0.0 && oldValue[0] != 0.0) {
        		
        		value = oldValue[0] +" + " + oldValue[1] + "i";
        		
        	} else {
        		value = "0.0";
        	}
			
			
		} else if (value instanceof String && value.toString().equals("")){
			value = "0.0";
		}
		super.setValue(value);
	}
	
}