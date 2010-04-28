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

public class SwingScilabVariableEditor extends SwingScilabTab implements Tab, SimpleVariableEditor {

	private SwingEditvarTableModel<Object> dataModel;
	private JTable table;

	/**
	 * Create a JTable with data Model.
	 * @param data : The JTable data.
	 */
	public SwingScilabVariableEditor(Object[][] data) {
		super(Messages.gettext("Variable Browser"));

		dataModel = new SwingEditvarTableModel<Object>(data);

		table = new JTable(dataModel);
		table.setFillsViewportHeight(true);
		table.setAutoResizeMode(CENTER);
		table.setAutoCreateRowSorter(true);

		// Mouse selection mode
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		JScrollPane scrollPane = new JScrollPane(table);
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
		dataModel.setData(data);
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
