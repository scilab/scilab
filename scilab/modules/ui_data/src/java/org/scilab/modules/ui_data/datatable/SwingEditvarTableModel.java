package org.scilab.modules.ui_data.datatable;

import javax.swing.table.AbstractTableModel;


/**
 * Swing implementation of table model.
 * @param <Type>
 */
public class SwingEditvarTableModel<Type> extends AbstractTableModel implements ScilabTable<Type> {

	private static final long serialVersionUID = -4255704246347716837L;

	private Type[][] data;

	/**
	 * Default construction setting data.
	 * @param data : the data to fill the Jtabe with.
	 */
	public SwingEditvarTableModel(Type[][] data) {
		this.data = data;
	}

	/**
	 * {@inheritDoc}
	 */
	public int getColumnCount() {
			return data[0].length;
	}

	/**
	 * {@inheritDoc}
	 */
	public int getRowCount() {
		if (data == null) {
			return 0;
		}
		return data.length;
	}

	/**
	 * {@inheritDoc}
	 */
	public Type getValueAt(int row, int col) {
		if (data == null) {
			return null;
		}
		return data[row][col];
	}

	/**
	 * {@inheritDoc}
	 */
	public void setData(Type[][] data) {
		this.data = data;
	}

	/**
	 * {@inheritDoc}
	 */
	public String getColumnName(int col) {
		return String.valueOf(col);
	}

}
