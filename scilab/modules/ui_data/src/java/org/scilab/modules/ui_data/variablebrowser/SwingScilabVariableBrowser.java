/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

import java.awt.Color;
import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.RowFilter;
import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.ui_data.BrowseVar;
import org.scilab.modules.ui_data.actions.BooleanFilteringAction;
import org.scilab.modules.ui_data.actions.CompiledFunctionFilteringAction;
import org.scilab.modules.ui_data.actions.DoubleFilteringAction;
import org.scilab.modules.ui_data.actions.FunctionLibFilteringAction;
import org.scilab.modules.ui_data.actions.GraphicHandlesFilteringAction;
import org.scilab.modules.ui_data.actions.HelpAction;
import org.scilab.modules.ui_data.actions.ImplicitPolynomialFilteringAction;
import org.scilab.modules.ui_data.actions.IntegerFilteringAction;
import org.scilab.modules.ui_data.actions.IntrinsicFunctionFilteringAction;
import org.scilab.modules.ui_data.actions.ListFilteringAction;
import org.scilab.modules.ui_data.actions.MListFilteringAction;
import org.scilab.modules.ui_data.actions.MatlabSparseFilteringAction;
import org.scilab.modules.ui_data.actions.PointerFilteringAction;
import org.scilab.modules.ui_data.actions.PolynomialFilteringAction;
import org.scilab.modules.ui_data.actions.ScilabVarFilteringAction;
import org.scilab.modules.ui_data.actions.ScilabVarFilteringButtonAction;
import org.scilab.modules.ui_data.actions.SparseBoolFilteringAction;
import org.scilab.modules.ui_data.actions.SparseFilteringAction;
import org.scilab.modules.ui_data.actions.StringFilteringAction;
import org.scilab.modules.ui_data.actions.TListFilteringAction;
import org.scilab.modules.ui_data.actions.UncompiledFunctionFilteringAction;
import org.scilab.modules.ui_data.datatable.SwingTableModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.actions.ExportToCsvAction;
import org.scilab.modules.ui_data.variableeditor.actions.PlotAction;
import org.scilab.modules.ui_data.variablebrowser.actions.CloseAction;
import org.scilab.modules.ui_data.variablebrowser.actions.DeleteAction;
import org.scilab.modules.ui_data.variablebrowser.actions.ModifyAction;
import org.scilab.modules.ui_data.variablebrowser.actions.RefreshAction;
import org.scilab.modules.ui_data.variablebrowser.rowfilter.VariableBrowserRowDataFilter;
import org.scilab.modules.ui_data.variablebrowser.rowfilter.VariableBrowserRowTypeFilter;

/**
 * Swing implementation of Scilab Variable browser
 * uses JTable
 */
public final class SwingScilabVariableBrowser extends SwingScilabDockablePanel implements SimpleVariableBrowser {

    public static final String VARBROWSERUUID = "3b649047-6a71-4998-bd8e-00d367a4793c";

    private static final long serialVersionUID = 2169382559550113917L;

    private SwingTableModel<Object> dataModel;
    private JTable table;

    private MenuBar menuBar;
    private Menu fileMenu;
    private Menu helpMenu;
    private Menu filterMenu;
    private CheckBoxMenuItem filterDoubleCheckBox;
    private CheckBoxMenuItem filterPolynomialCheckBox;
    private CheckBoxMenuItem filterScilabVarCheckBox;
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
    private JButton filteringButton;

    private boolean isSetData = false;

    private TableRowSorter < ? > rowSorter;

    /**
     * Create a JTable with data Model.
     * @param columnsName : Titles of JTable columns.
     */
    public SwingScilabVariableBrowser(String[] columnsName, int[] aligment) {
        super(UiDataMessages.VARIABLE_BROWSER, VARBROWSERUUID);

        setAssociatedXMLIDForHelp("browsevar");

        buildMenuBar();
        addMenuBar(menuBar);

        ToolBar toolBar = ScilabToolBar.createToolBar();
        SwingScilabToolBar stb = (SwingScilabToolBar) toolBar.getAsSimpleToolBar();
        stb.add(RefreshAction.createButton(UiDataMessages.REFRESH));
        stb.addSeparator();
        stb.add(ModifyAction.createButton(this, UiDataMessages.MODIFY));
        stb.add(DeleteAction.createButton(this, UiDataMessages.DELETE));
        stb.addSeparator();
        stb.add(HelpAction.createButton(UiDataMessages.HELP));
        filteringButton = ScilabVarFilteringButtonAction.createButton("Show/hide Scilab variable");
        //        toolBar.add(filteringButton);
        addToolBar(toolBar);

        dataModel = new SwingTableModel<Object>(columnsName);

        table = new JTable(dataModel) {
            //Implement table cell tool tips.
            public String getToolTipText(MouseEvent e) {
                String tip = null;
                TableModel model = ((JTable) e.getSource()).getModel();
                java.awt.Point p = e.getPoint();
                int rowIndex = rowAtPoint(p);

                if (rowIndex >= 0) {
                    rowIndex = convertRowIndexToModel(rowIndex);
                    int colIndex = columnAtPoint(p);
                    if (colIndex == BrowseVar.TYPE_DESC_COLUMN_INDEX) { /* Scilab type */
                        try {
                            tip = Messages.gettext("Scilab type:") + " " + model.getValueAt(rowIndex, BrowseVar.TYPE_COLUMN_INDEX).toString();
                        } catch (IllegalArgumentException exception) {
                            /* If the type is not known/managed, don't crash */
                        }
                    } else {

                        if (colIndex == BrowseVar.SIZE_COLUMN_INDEX) {
                            /* Use the getModel() method because the
                             * column 5 has been removed from display
                             * but still exist in the model */
                            tip = Messages.gettext("Bytes:") + " " + model.getValueAt(rowIndex, BrowseVar.BYTES_COLUMN_INDEX).toString();
                        }
                    }
                }
                return tip;
            }
        };

        table.setFillsViewportHeight(true);
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        table.setAutoCreateRowSorter(true);

        /* Size of the icon column */
        table.getColumnModel().getColumn(0).setPreferredWidth(30);

        /* Hide the columns. But keep it in memory for the tooltip */
        TableColumn column = table.getColumnModel().getColumn(BrowseVar.NB_COLS_INDEX);
        table.removeColumn(column);

        /* The order to removing does matter since it changes the positions */

        column = table.getColumnModel().getColumn(BrowseVar.NB_ROWS_INDEX);
        table.removeColumn(column);

        column = table.getColumnModel().getColumn(BrowseVar.TYPE_COLUMN_INDEX);
        table.removeColumn(column);

        column = table.getColumnModel().getColumn(BrowseVar.FROM_SCILAB_COLUMN_INDEX);
        table.removeColumn(column);

        column = table.getColumnModel().getColumn(BrowseVar.BYTES_COLUMN_INDEX);
        table.removeColumn(column);

        table.addMouseListener(new BrowseVarMouseListener());
        // Mouse selection mode
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        table.setCellSelectionEnabled(true);

        table.setBackground(Color.WHITE);
        if (table.getGridColor().equals(Color.WHITE)) {
            table.setGridColor(new Color(128, 128, 128));
        }
        table.setShowHorizontalLines(true);
        table.setShowVerticalLines(true);

        for (int i = 0; i < aligment.length; i++) {
            align(table, columnsName[i], aligment[i]);
        }

        // Plug the shortcuts
        ExportToCsvAction.registerAction(this, table);

        JScrollPane scrollPane = new JScrollPane(table);
        setContentPane(scrollPane);
        WindowsConfigurationManager.restorationFinished(this);
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
        isSetData = true;
        dataModel.setData(data);
        rowSorter = new TableRowSorter<TableModel>(dataModel);
        this.updateRowFiltering();
    }

    private static void align(JTable table, String name, int alignment) {
        if (alignment != -1) {
            DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
            renderer.setHorizontalAlignment(alignment);
            try {
                TableColumn col = table.getColumn(name);
                if (col != null) {
                    col.setCellRenderer(renderer);
                }
            } catch (IllegalArgumentException e) { }
        }
    }

    /**
     * Update the display after filtering
     * @see org.scilab.modules.ui_data.variablebrowser.SimpleVariableBrowser#updateRowFiltering()
     */
    public void updateRowFiltering() {
        if (isSetData == false) { /* If the pull of the data has not been done, do it (please) */
            try {
                ScilabInterpreterManagement.synchronousScilabExec("browsevar");
                isSetData = true;
            } catch (InterpreterException e) {
                System.err.println(e);
            }
        }
        VariableBrowserRowTypeFilter rowTypeFilter = new VariableBrowserRowTypeFilter(getFilteredTypeValues());
        VariableBrowserRowDataFilter rowDataFilter = new VariableBrowserRowDataFilter(getFilteredDataValues());

        List<RowFilter<Object, Object>> filters = new ArrayList<RowFilter<Object, Object>>();
        RowFilter<Object, Object> compoundRowFilter = null;
        filters.add(rowTypeFilter);
        filters.add(rowDataFilter);
        compoundRowFilter = RowFilter.andFilter(filters);

        rowSorter.setRowFilter(compoundRowFilter);
        table.setRowSorter(rowSorter);
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

            // Right click management
            if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e)) || e.isPopupTrigger()) {


                int clickedRow = ((JTable) e.getSource()).rowAtPoint(e.getPoint());
                // Does nothing if no variable selected
                if (clickedRow != -1) {
                    // Right click ? We force the selection of the current row
                    table.setRowSelectionInterval(clickedRow, clickedRow);
                    table.setEditingRow(clickedRow);

                    displayContextMenu();
                }
            }

            if (e.getClickCount() >= 2) {
                int clickedRow = ((JTable) e.getSource()).rowAtPoint(e.getPoint());
                if (clickedRow != -1) {
                    String variableName = table.getValueAt(clickedRow, BrowseVar.NAME_COLUMN_INDEX).toString();

                    String variableVisibility = ((JTable) e.getSource())
                                                .getValueAt(((JTable) e.getSource()).getSelectedRow(), BrowseVar.VISIBILITY_COLUMN_INDEX).toString();
                    SwingScilabVariableBrowser.this.startEditVar(variableVisibility, variableName);
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

        /**
         * Create and display the context menu
         */
        private void displayContextMenu() {
            ContextMenu menu = ScilabContextMenu.createContextMenu();
            ModifyAction modify = new ModifyAction(SwingScilabVariableBrowser.this);
            menu.add(modify.createMenuItem());

            DeleteAction delete = new DeleteAction(SwingScilabVariableBrowser.this);
            menu.add(delete.createMenuItem());

            menu.add(ExportToCsvAction.createMenuItem(SwingScilabVariableBrowser.this, UiDataMessages.EXPORTCSV));

            Menu menuPlot = ScilabMenu.createMenu();
            menuPlot.setText(UiDataMessages.PLOTALL);
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "plot2d", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "Matplot", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "grayplot", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "Sgrayplot", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "champ", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "histplot", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "mesh", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "surf", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "hist3d", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "contour2d", false));
            menuPlot.add(PlotAction.createMenuItem((SwingScilabDockablePanel)SwingScilabVariableBrowser.this, "pie", false));
            menu.add(menuPlot);

            menu.setVisible(true);

            ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
                MouseInfo.getPointerInfo().getLocation().x,
                MouseInfo.getPointerInfo().getLocation().y);

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
        fileMenu.add(RefreshAction.createMenuItem(UiDataMessages.REFRESH));
        fileMenu.addSeparator();
        fileMenu.add(CloseAction.createMenu());

        menuBar.add(fileMenu);

        filterMenu = ScilabMenu.createMenu();
        filterMenu.setText(UiDataMessages.FILTER);
        filterMenu.setMnemonic('I');

        filterScilabVarCheckBox = ScilabVarFilteringAction.createCheckBoxMenu();
        filterScilabVarCheckBox.setChecked(true);
        filterMenu.add(filterScilabVarCheckBox);

        filterMenu.addSeparator();

        filterBooleanCheckBox = BooleanFilteringAction.createCheckBoxMenu();
        filterBooleanCheckBox.setChecked(true);
        filterMenu.add(filterBooleanCheckBox);

        filterDoubleCheckBox = DoubleFilteringAction.createCheckBoxMenu();
        filterDoubleCheckBox.setChecked(true);
        filterMenu.add(filterDoubleCheckBox);

        filterPolynomialCheckBox = PolynomialFilteringAction.createCheckBoxMenu();
        filterPolynomialCheckBox.setChecked(true);
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
        filterGraphicHandlesCheckBox.setChecked(true);
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

        helpMenu = ScilabMenu.createMenu();
        helpMenu.setText("?");
        helpMenu.setMnemonic('?');
        helpMenu.add(HelpAction.createMenuItem(UiDataMessages.HELP));
        menuBar.add(helpMenu);
    }


    /**
     * Filter management of data (Scilab or user data)
     * @return the set of filtered values
     */
    public boolean getFilteredDataValues() {
        return filterScilabVarCheckBox.isChecked();

    }

    /**
     * Filter management of type
     * @return the set of filtered values
     */
    public HashSet<ScilabTypeEnum> getFilteredTypeValues() {
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
     * Get this browser as a Tab object
     * @return the tab
     */
    public SwingScilabDockablePanel getBrowserTab() {
        return this;
    }


    /**
     * Return the variable Browser Table
     * @return The variable Browser Table
     */
    public JTable getTable() {
        return table;
    }

    /**
     * Start the variable editor
     *
     * @param variableVisibility the visibility of the variable
     * @param variableName The variable name
     */
    public void startEditVar(String variableVisibility, String variableName) {
        // Global variables are not editable yet
        if (variableVisibility.equals("global")) {
            ScilabModalDialog.show(getBrowserTab(),
                                   UiDataMessages.GLOBAL_NOT_EDITABLE,
                                   UiDataMessages.VARIABLE_EDITOR,
                                   IconType.ERROR_ICON);
            return;
        }

        try {
            asynchronousScilabExec(null,
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
