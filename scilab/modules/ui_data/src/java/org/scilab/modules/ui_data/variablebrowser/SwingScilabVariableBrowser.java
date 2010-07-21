/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.HashSet;

import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.actions.BooleanFilteringAction;
import org.scilab.modules.ui_data.actions.CompiledFunctionFilteringAction;
import org.scilab.modules.ui_data.actions.DoubleFilteringAction;
import org.scilab.modules.ui_data.actions.FunctionLibFilteringAction;
import org.scilab.modules.ui_data.actions.GraphicHandlesFilteringAction;
import org.scilab.modules.ui_data.actions.IntegerFilteringAction;
import org.scilab.modules.ui_data.actions.ListFilteringAction;
import org.scilab.modules.ui_data.actions.MListFilteringAction;
import org.scilab.modules.ui_data.actions.PointerFilteringAction;
import org.scilab.modules.ui_data.actions.PolynomialFilteringAction;
import org.scilab.modules.ui_data.actions.SparceBoolFilteringAction;
import org.scilab.modules.ui_data.actions.SparceFilteringAction;
import org.scilab.modules.ui_data.actions.StringFilteringAction;
import org.scilab.modules.ui_data.actions.TListFilteringAction;
import org.scilab.modules.ui_data.actions.UncompiledFunctionFilteringAction;
import org.scilab.modules.ui_data.datatable.SwingTableModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variablebrowser.actions.CloseAction;
import org.scilab.modules.ui_data.variablebrowser.rowfilter.VariableBrowserRowFilter;

/**
 * Swing implementation of Scilab Variable browser
 * uses JTable
 */
public final class SwingScilabVariableBrowser extends SwingScilabTab implements Tab, SimpleVariableBrowser {

	private static final long serialVersionUID = 2169382559550113917L;

	private static final int DOUBLE_CLASS_NUMBER = 1;
	private static final int POLYNOMIAL_CLASS_NUMBER = 2;
	private static final int BOOLEAN_CLASS_NUMBER = 4;
	private static final int SPARCE_CLASS_NUMBER = 5;
	private static final int SPARCE_BOOLEAN_CLASS_NUMBER = 6;
	private static final int INTEGER_CLASS_NUMBER = 6;
	private static final int GRAPHIC_HANDLES_CLASS_NUMBER = 6;
	private static final int STRING_CLASS_NUMBER = 10;
	private static final int UNCOMPILED_FUNCTION_CLASS_NUMBER = 11;
	private static final int COMPILED_FUNCTION_CLASS_NUMBER = 13;
	private static final int FUNCTIONLIB_CLASS_NUMBER = 14;
	private static final int LIST_CLASS_NUMBER = 15;
	private static final int TLIST_CLASS_NUMBER = 16;
	private static final int MLIST_CLASS_NUMBER = 17;
	private static final int POINTER_CLASS_NUMBER = 128;

	private SwingTableModel<Object> dataModel;
	private JTable table;
	private VariableBrowserRowFilter rowFilter;

	private MenuBar menuBar;
	private Menu fileMenu;
	private Menu filterMenu;
	private CheckBoxMenuItem filterDoubleCheckBox;
	private CheckBoxMenuItem filterPolynomialCheckBox;
	private CheckBoxMenuItem filterBooleanCheckBox;
	private CheckBoxMenuItem filterSparceCheckBox;
	private CheckBoxMenuItem filterSparceBoolCheckBox;
	private CheckBoxMenuItem filterIntegerCheckBox;
	private CheckBoxMenuItem filterGraphicHandlesCheckBox;
	private CheckBoxMenuItem filterStringCheckBox;
	private CheckBoxMenuItem filterUncompiledFuncCheckBox;
	private CheckBoxMenuItem filtercompiledFuncCheckBox;
	private CheckBoxMenuItem filterFunctionLibCheckBox;
	private CheckBoxMenuItem filterListCheckBox;
	private CheckBoxMenuItem filterTListCheckBox;
	private CheckBoxMenuItem filterMListCheckBox;
	private CheckBoxMenuItem filterPointerCheckBox;
	
	private TableRowSorter< ? > rowSorter; 

	/**
	 * Create a JTable with data Model.
	 * @param columnsName : Titles of JTable columns.
	 */
	public SwingScilabVariableBrowser(String[] columnsName) {
		super(Messages.gettext("Variable Browser"));

		buildMenuBar();

		addMenuBar(menuBar);

		dataModel = new SwingTableModel<Object>(columnsName);

		table = new JTable(dataModel);
		table.setFillsViewportHeight(true);
		table.setAutoResizeMode(CENTER);
		table.setAutoCreateRowSorter(true);

		table.addMouseListener(new BrowseVarMouseListener());
		// Mouse selection mode
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		table.setCellSelectionEnabled(true);


		table.setBackground(Color.WHITE);

		JScrollPane scrollPane = new JScrollPane(table);
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
	public void setColumnNames(String[] columnNames) {
		dataModel.setColumnNames(columnNames);
	}

	/**
	 * {@inheritDoc}
	 */
	public void setData(Object[][] data) {
		dataModel.setData(data);
		HashSet<Integer> filteredValues = getFilteredValues();
		rowSorter = new TableRowSorter<TableModel>(dataModel);
		rowFilter = new VariableBrowserRowFilter(filteredValues);
		rowSorter.setRowFilter(rowFilter);
		table.setRowSorter(rowSorter);
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


	/**
	 * MouseListener inner class
	 */
	private class BrowseVarMouseListener implements MouseListener {

		/**
		 * Contructor
		 */
		public BrowseVarMouseListener() {

		}

		/**
		 * Mouse CLICKED event handling
		 * @param e the event
		 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
		 */
		public void mouseClicked(MouseEvent e) {

			if (e.getClickCount() >= 2) {

				String variableName = ((JTable) e.getSource()).getValueAt(((JTable) e.getSource()).getSelectedRow(), 1).toString();
				final ActionListener action = new ActionListener() {

					public void actionPerformed(ActionEvent e) {

					}
				};

				try {
					asynchronousScilabExec(action, "try "
								+ "editvar(\"" + variableName + "\"); " 
								+ "catch "
								+ "messagebox(\"Variables of type \"\"\" + typeof (" 
								+ variableName + ") + \"\"\" can not be edited.\""
								+ ",\"Variable editor\", \"error\", \"modal\");"
								+ "end");
				} catch (InterpreterException e1) {
					System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage()); 
				}

			}

		}

		/**
		 * Mouse ENTERED event handling
		 * @param e the event
		 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
		 */
		public void mouseEntered(MouseEvent e) {
		}

		/**
		 * Mouse EXITED event handling
		 * @param e the event
		 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
		 */
		public void mouseExited(MouseEvent e) {
		}

		/**
		 * Mouse PRESSED event handling
		 * @param e the event
		 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
		 */
		public void mousePressed(MouseEvent e) {
		}

		/**
		 * Mouse RELEASED event handling
		 * @param e the event
		 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
		 */
		public void mouseReleased(MouseEvent e) {

		}
	}

	/**
	 * Creates the menuBar
	 */
	public void buildMenuBar() {
		menuBar = ScilabMenuBar.createMenuBar();

		fileMenu = ScilabMenu.createMenu();
		fileMenu.setText(UiDataMessages.FILE);
		fileMenu.setMnemonic('F');
		fileMenu.add(CloseAction.createMenu());

		menuBar.add(fileMenu);

		filterMenu = ScilabMenu.createMenu();
		filterMenu.setText(UiDataMessages.FILTER);
		
		filterBooleanCheckBox = BooleanFilteringAction.createCheckBoxMenu();
		filterBooleanCheckBox.setChecked(true);
		filterMenu.add(filterBooleanCheckBox);
		
		filterDoubleCheckBox = DoubleFilteringAction.createCheckBoxMenu();
		filterDoubleCheckBox.setChecked(true);
		filterMenu.add(filterDoubleCheckBox);
		
		filterPolynomialCheckBox = PolynomialFilteringAction.createCheckBoxMenu();
		filterBooleanCheckBox.setChecked(true);
		filterMenu.add(filterPolynomialCheckBox);
		
		filterFunctionLibCheckBox = FunctionLibFilteringAction.createCheckBoxMenu();
		filterMenu.add(filterFunctionLibCheckBox);
		
		filterStringCheckBox = StringFilteringAction.createCheckBoxMenu();
		filterStringCheckBox.setChecked(true);
		filterMenu.add(filterStringCheckBox);
		
		filterSparceCheckBox = SparceFilteringAction.createCheckBoxMenu();
		filterSparceCheckBox.setChecked(true);
		filterMenu.add(filterSparceCheckBox);
		
		filterSparceBoolCheckBox = SparceBoolFilteringAction.createCheckBoxMenu();
		filterSparceBoolCheckBox.setChecked(true);
		filterMenu.add(filterSparceBoolCheckBox);
		
		filterIntegerCheckBox = IntegerFilteringAction.createCheckBoxMenu();
		filterIntegerCheckBox.setChecked(true);
		filterMenu.add(filterIntegerCheckBox);
		
		filterGraphicHandlesCheckBox = GraphicHandlesFilteringAction.createCheckBoxMenu();
		filterMenu.add(filterGraphicHandlesCheckBox);
		
		filterUncompiledFuncCheckBox = UncompiledFunctionFilteringAction.createCheckBoxMenu();
		filterMenu.add(filterUncompiledFuncCheckBox);
		
		filtercompiledFuncCheckBox = CompiledFunctionFilteringAction.createCheckBoxMenu();
		filterMenu.add(filtercompiledFuncCheckBox);
		
		filterListCheckBox = ListFilteringAction.createCheckBoxMenu();
		filterListCheckBox.setChecked(true);
		filterMenu.add(filterListCheckBox);
		
		filterTListCheckBox = TListFilteringAction.createCheckBoxMenu();
		filterTListCheckBox.setChecked(true);
		filterMenu.add(filterTListCheckBox);

		filterMListCheckBox = MListFilteringAction.createCheckBoxMenu();
		filterMListCheckBox.setChecked(true);
		filterMenu.add(filterMListCheckBox);
		
		filterPointerCheckBox = PointerFilteringAction.createCheckBoxMenu();
		filterMenu.add(filterPointerCheckBox);

		menuBar.add(filterMenu);
	}


	/**
	 * Filter management
	 * @return the set of filtered values
	 */
	public HashSet<Integer> getFilteredValues() {
		HashSet<Integer> filteredValues = new HashSet<Integer>();
		// TODO to replace later by something which smells less
		if (!filterBooleanCheckBox.isChecked()) {
			filteredValues.add(BOOLEAN_CLASS_NUMBER);
		}

		if (!filterDoubleCheckBox.isChecked()) {
			filteredValues.add(DOUBLE_CLASS_NUMBER);
		}

		if (!filterFunctionLibCheckBox.isChecked()) {
			filteredValues.add(FUNCTIONLIB_CLASS_NUMBER);
		}

		if (!filterStringCheckBox.isChecked()) {
			filteredValues.add(STRING_CLASS_NUMBER);
		}

		if (!filterPolynomialCheckBox.isChecked()) {
			filteredValues.add(POLYNOMIAL_CLASS_NUMBER);
		}

		if (!filterSparceCheckBox.isChecked()) {
			filteredValues.add(SPARCE_CLASS_NUMBER);
		}

		if (!filterSparceBoolCheckBox.isChecked()) {
			filteredValues.add(SPARCE_BOOLEAN_CLASS_NUMBER);
		}
		
		if (!filterIntegerCheckBox.isChecked()) {
			filteredValues.add(INTEGER_CLASS_NUMBER);
		}
		
		if (!filterGraphicHandlesCheckBox.isChecked()) {
			filteredValues.add(GRAPHIC_HANDLES_CLASS_NUMBER);
		}
		
		if (!filterUncompiledFuncCheckBox.isChecked()) {
			filteredValues.add(UNCOMPILED_FUNCTION_CLASS_NUMBER);
		}
		
		if (!filtercompiledFuncCheckBox.isChecked()) {
			filteredValues.add(COMPILED_FUNCTION_CLASS_NUMBER);
		}
		
		if (!filterListCheckBox.isChecked()) {
			filteredValues.add(LIST_CLASS_NUMBER);
		}
		
		if (!filterTListCheckBox.isChecked()) {
			filteredValues.add(TLIST_CLASS_NUMBER);
		}
		
		if (!filterMListCheckBox.isChecked()) {
			filteredValues.add(MLIST_CLASS_NUMBER);
		}
		
		if (!filterPointerCheckBox.isChecked()) {
			filteredValues.add(POINTER_CLASS_NUMBER);
		}
		
		return filteredValues;
	}

	/**
	 * Update the display after filtering
	 * @see org.scilab.modules.ui_data.variablebrowser.SimpleVariableBrowser#updateRowFiltering()
	 */
	public void updateRowFiltering() {
		rowFilter = new VariableBrowserRowFilter(getFilteredValues());
		rowSorter.setRowFilter(rowFilter);
		table.setRowSorter(rowSorter);
	}
}
