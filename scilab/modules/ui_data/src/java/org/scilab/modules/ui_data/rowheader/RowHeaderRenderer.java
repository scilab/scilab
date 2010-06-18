package org.scilab.modules.ui_data.rowheader;

import java.awt.Component;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JTable;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;
import javax.swing.table.JTableHeader;



public class RowHeaderRenderer extends JButton implements ListCellRenderer {

	public RowHeaderRenderer() {
		super();
	}

	public Component getListCellRendererComponent( JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
		if (value == null) {
			setText("");
		} else {
			setText( value.toString());
		}
		return this;
	}
}

