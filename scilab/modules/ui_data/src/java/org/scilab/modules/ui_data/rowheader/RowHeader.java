package org.scilab.modules.ui_data.rowheader;

import javax.swing.JList;
import javax.swing.JTable;

public class RowHeader extends JList {

	public RowHeader (RowHeaderModel rowHeaderModel, JTable table) {
		super(rowHeaderModel);    
		setFixedCellWidth(50);
		setFixedCellHeight(table.getRowHeight()
				+ table.getRowMargin()
				- table.getIntercellSpacing().height);
		setCellRenderer(new RowHeaderRenderer());
	}

}
