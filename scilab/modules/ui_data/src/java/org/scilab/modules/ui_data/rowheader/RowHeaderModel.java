package org.scilab.modules.ui_data.rowheader;

import javax.swing.AbstractListModel;
import javax.swing.table.AbstractTableModel;

public class RowHeaderModel extends AbstractListModel {
	int size;
	
	public RowHeaderModel (AbstractTableModel tableModel) {
		size = tableModel.getRowCount();
	}
	
    public int getSize() {
    	return this.size;
    }
    
    public Object getElementAt(int index) {
    	if (index > size) {
    		size = index;
    	}
      return (Integer) index+1;
    }
}
