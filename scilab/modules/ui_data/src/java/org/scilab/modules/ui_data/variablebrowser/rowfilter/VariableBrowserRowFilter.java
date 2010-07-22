package org.scilab.modules.ui_data.variablebrowser.rowfilter;

import java.util.HashSet;

import javax.swing.RowFilter;

import org.scilab.modules.types.scilabTypes.ScilabTypeEnum;

public class VariableBrowserRowFilter extends RowFilter<Object, Object>{

	private static final int TYPE_INDEX = 3;
	
	private HashSet<ScilabTypeEnum> filteredValues;
	
	public VariableBrowserRowFilter() {
		super();
		filteredValues = new HashSet<ScilabTypeEnum>();
	}
	
	public VariableBrowserRowFilter(HashSet<ScilabTypeEnum> filteredValues) {
		this.filteredValues = filteredValues;
	}
	
	@Override
	public boolean include(Entry< ? extends Object, ? extends Object> entry) {
		Integer currentRowVariableclassNumber =  (Integer) entry.getValue(TYPE_INDEX);
		
		try {
			if (filteredValues.contains(ScilabTypeEnum.swigToEnum(currentRowVariableclassNumber))) {
				return false;
			}
		} catch (IllegalArgumentException e) {
			// This case should never occur
			// But there is a bug in Scilab global variables type detection
			// So we show these "unknown type" variables by default (See bug #7333)
			return true;
		}
		
		return true;
	}

}
