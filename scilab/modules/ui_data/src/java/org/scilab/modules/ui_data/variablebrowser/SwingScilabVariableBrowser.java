/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
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
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.ui_data.BrowseVar;
import org.scilab.modules.ui_data.actions.BooleanFilteringAction;
import org.scilab.modules.ui_data.actions.CompiledFunctionFilteringAction;
import org.scilab.modules.ui_data.actions.DoubleFilteringAction;
import org.scilab.modules.ui_data.actions.FunctionLibFilteringAction;
import org.scilab.modules.ui_data.actions.GraphicHandlesFilteringAction;
import org.scilab.modules.ui_data.actions.ImplicitPolynomialFilteringAction;
import org.scilab.modules.ui_data.actions.IntegerFilteringAction;
import org.scilab.modules.ui_data.actions.IntrinsicFunctionFilteringAction;
import org.scilab.modules.ui_data.actions.ListFilteringAction;
import org.scilab.modules.ui_data.actions.MListFilteringAction;
import org.scilab.modules.ui_data.actions.MatlabSparseFilteringAction;
import org.scilab.modules.ui_data.actions.PointerFilteringAction;
import org.scilab.modules.ui_data.actions.PolynomialFilteringAction;
import org.scilab.modules.ui_data.actions.SparseBoolFilteringAction;
import org.scilab.modules.ui_data.actions.SparseFilteringAction;
import org.scilab.modules.ui_data.actions.StringFilteringAction;
import org.scilab.modules.ui_data.actions.TListFilteringAction;
import org.scilab.modules.ui_data.actions.UncompiledFunctionFilteringAction;
import org.scilab.modules.ui_data.datatable.SwingTableModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variablebrowser.actions.CloseAction;
import org.scilab.modules.ui_data.variablebrowser.actions.RefreshAction;
import org.scilab.modules.ui_data.variablebrowser.rowfilter.VariableBrowserRowFilter;

/**
 * Swing implementation of Scilab Variable browser
 * uses JTable
 */
public final class SwingScilabVariableBrowser extends SwingScilabTab implements Tab, SimpleVariableBrowser {

    private static final long serialVersionUID = 2169382559550113917L;

    private SwingTableModel<Object> dataModel;
    private JTable table;
    private VariableBrowserRowFilter rowFilter;

    private MenuBar menuBar;
    private Menu fileMenu;
    private Menu filterMenu;
    private CheckBoxMenuItem filterDoubleCheckBox;
    private CheckBoxMenuItem filterPolynomialCheckBox;
    private CheckBoxMenuItem filterBooleanCheckBox;
    private CheckBoxMenuItem filterSparseCheckBox;
    private CheckBoxMenuItem filterSparseBoolCheckBox;
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
    private CheckBoxMenuItem filterIntrinsicFunctionCheckBox;
    private CheckBoxMenuItem filterMatlabSparseCheckBox;
    private CheckBoxMenuItem filterImplicitPolynomialCheckBox;

    private TableRowSorter< ? > rowSorter;

    /**
     * Create a JTable with data Model.
     * @param columnsName : Titles of JTable columns.
     */
    public SwingScilabVariableBrowser(String[] columnsName) {
        super(UiDataMessages.VARIABLE_BROWSER);

        buildMenuBar();

        addMenuBar(menuBar);

        ToolBar toolBar = ScilabToolBar.createToolBar();
        toolBar.add(RefreshAction.createButton(UiDataMessages.REFRESH));
        addToolBar(toolBar);

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
        HashSet<ScilabTypeEnum> filteredValues = getFilteredValues();
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
    /**
     * {@inheritDoc}
     */
    public Window getParentWindow() {
        return (Window) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
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

                String variableVisibility = ((JTable) e.getSource())
                    .getValueAt(((JTable) e.getSource()).getSelectedRow(), BrowseVar.VISIBILITY_COLUMN_INDEX).toString();

                // Global variables are not editable yet
                if (variableVisibility.equals("global")) {
                    ScilabModalDialog.show(getBrowserTab(),
                                           UiDataMessages.GLOBAL_NOT_EDITABLE,
                                           UiDataMessages.VARIABLE_EDITOR,
                                           IconType.ERROR_ICON);
                    return;
                }

                try {
                    asynchronousScilabExec(action,
                                           "if exists(\"" + variableName + "\") == 1 then "
                                           + "  try "
                                           + "    editvar(\"" + variableName + "\"); "
                                           + "  catch "
                                           + "    messagebox(\"Variables of type \"\"\" + typeof ("
                                           + variableName + ") + \"\"\" can not be edited.\""
                                           + ",\"" + UiDataMessages.VARIABLE_EDITOR + "\", \"error\", \"modal\");"
                                           + "    clear ans;"   // clear return value of messagebox
                                           + "  end "
                                           + "else "
                                           + "  messagebox(\"Variable \"\""
                                           + variableName + "\"\" no more exists.\""
                                           + ",\"" + UiDataMessages.VARIABLE_EDITOR + "\", \"error\", \"modal\");"
                                           + "  clear ans;"  // clear return value of messagebox
                                           + "  browsevar();" // Reload browsevar to remove cleared variables
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

        filterSparseCheckBox = SparseFilteringAction.createCheckBoxMenu();
        filterSparseCheckBox.setChecked(true);
        filterMenu.add(filterSparseCheckBox);

        filterSparseBoolCheckBox = SparseBoolFilteringAction.createCheckBoxMenu();
        filterSparseBoolCheckBox.setChecked(true);
        filterMenu.add(filterSparseBoolCheckBox);

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
        filterPointerCheckBox.setChecked(true);
        filterMenu.add(filterPointerCheckBox);

        filterIntrinsicFunctionCheckBox = IntrinsicFunctionFilteringAction.createCheckBoxMenu();
        filterMenu.add(filterIntrinsicFunctionCheckBox);

        filterMatlabSparseCheckBox = MatlabSparseFilteringAction.createCheckBoxMenu();
        filterMenu.add(filterMatlabSparseCheckBox);

        filterImplicitPolynomialCheckBox = ImplicitPolynomialFilteringAction.createCheckBoxMenu();
        filterMenu.add(filterImplicitPolynomialCheckBox);

        menuBar.add(filterMenu);
    }


    /**
     * Filter management
     * @return the set of filtered values
     */
    public HashSet<ScilabTypeEnum> getFilteredValues() {
        HashSet<ScilabTypeEnum> filteredValues = new HashSet<ScilabTypeEnum>();
        // TODO to replace later by something which smells less
        if (!filterBooleanCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_boolean);
        }

        if (!filterDoubleCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_matrix);
        }

        if (!filterFunctionLibCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_lib);
        }

        if (!filterStringCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_strings);
        }

        if (!filterPolynomialCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_poly);
        }

        if (!filterSparseCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_sparse);
        }

        if (!filterSparseBoolCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_boolean_sparse);
        }

        if (!filterIntegerCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_ints);
        }

        if (!filterGraphicHandlesCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_handles);
        }

        if (!filterUncompiledFuncCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_u_function);
        }

        if (!filtercompiledFuncCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_c_function);
        }

        if (!filterListCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_list);
        }

        if (!filterTListCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_tlist);
        }

        if (!filterMListCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_mlist);
        }

        if (!filterPointerCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_pointer);
        }

        if (!filterIntrinsicFunctionCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_intrinsic_function);
        }

        if (!filterMatlabSparseCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_matlab_sparse);
        }

        if (!filterImplicitPolynomialCheckBox.isChecked()) {
            filteredValues.add(ScilabTypeEnum.sci_implicit_poly);
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

    /**
     * Get this browser as a Tab object
     * @return the tab
     */
    public Tab getBrowserTab() {
        return this;
    }
}
