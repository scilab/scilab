package org.scilab.modules.ui_data.variablebrowser.rowfilter;

import java.util.ArrayList;
import java.util.HashSet;

import javax.swing.RowFilter;

public class VariableBrowserRowFilter extends RowFilter<Object, Object>{

	private HashSet<Integer> filteredValues;
	
	public VariableBrowserRowFilter() {
		super();
		filteredValues = new HashSet<Integer>();

	}
	
	public VariableBrowserRowFilter(HashSet<Integer> filteredValues) {
		this.filteredValues = filteredValues;
	}
	

	
	@Override
	public boolean include(Entry<? extends Object, ? extends Object> entry) {
		Integer currentRowVariableclassNumber =  (Integer)entry.getValue(3);
		if (filteredValues.contains(currentRowVariableclassNumber)) {
			return false ;
		}
		
		return true;
	}

}
