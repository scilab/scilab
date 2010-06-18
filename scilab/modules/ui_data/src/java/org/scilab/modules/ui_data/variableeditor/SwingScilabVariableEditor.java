/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor;


import java.awt.Color;

import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.RowHeader;
import org.scilab.modules.ui_data.rowheader.RowHeaderModel;
import org.scilab.modules.ui_data.variableeditor.celleditor.CellEditorFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabBooleanCellEditor;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabDoubleCellEditor;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabStringCellEditor;
import org.scilab.modules.ui_data.variableeditor.celleditor.VariableEditorCellEditor;
import org.scilab.modules.ui_data.variableeditor.listeners.ExpandListener;
import org.scilab.modules.ui_data.variableeditor.renderers.RendererFactory;


/**
 * Swing implementation of Scilab Variable Editor
 * uses JTable
 */
public class SwingScilabVariableEditor extends SwingScilabTab implements Tab, SimpleVariableEditor {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private SwingEditvarTableModel<Object> dataModel;
	private JTable table;
	private JScrollPane scrollPane;

	/**
	 * Create a JTable with data Model.
	 * @param data : The JTable data.
	 */
	public SwingScilabVariableEditor(Object[][] data) {
		super(Messages.gettext("Variable Browser"));

		dataModel = new SwingEditvarTableModel<Object>(data, getDefaultValue(data));

		table = new JTable(dataModel);
		table.setDefaultEditor(Object.class, CellEditorFactory.createCellEditor(data));
		table.setFillsViewportHeight(true);
		table.setAutoResizeMode(CENTER);
		table.setRowHeight(25);
		table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(data));

		//table.getColumnModel().setColumnMargin(2);

		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		table.setCellSelectionEnabled(true);		

		RowHeaderModel rowHeaderModel = new RowHeaderModel(dataModel);
		RowHeader rowHeader = new RowHeader(rowHeaderModel, table);


		scrollPane = new JScrollPane(table);
		scrollPane.setRowHeaderView(rowHeader);
		scrollPane.getHorizontalScrollBar().addAdjustmentListener(new ExpandListener());

		table.setBackground(Color.WHITE);
		setContentPane(scrollPane);
	}

	/**
	 * {@inheritDoc}
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		setInfoBar(infoBarToAdd);
	}

	/**
	 * {@inheritDoc}
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		setMenuBar(menuBarToAdd);
	}

	/**
	 * {@inheritDoc}
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		setToolBar(toolBarToAdd);
	}

	/**
	 * {@inheritDoc}
	 */

	public void setData(Object[][] data) {
		dataModel.setDataVector(data);
	}

	/**
	 * {@inheritDoc}
	 */
	public void setValueAt(Object value, int row, int col) {
		dataModel.setValueAt(value, row, col);
		RowHeaderModel rowHeaderModel = new RowHeaderModel(dataModel);
		RowHeader rowHeader = new RowHeader(rowHeaderModel, table);
		scrollPane.setRowHeaderView(rowHeader);
	}

	public Object getDefaultValue(Object[][] data){

		if (data instanceof String[][]) {
			return "";
		} else if (data instanceof Double[][]) {
			return 0.0;
		} else if (data instanceof Boolean[][]) {
			return false;
		}
		return "";
	}

	/**
	 * {@inheritDoc}
	 */
	public SimpleTab getAsSimpleTab() {
		return this;
	}

	/**
	 * {@inheritDoc}
	 */
	public Window getParentWindow() {
		// TODO Auto-generated method stub
		return null;
	}

}
