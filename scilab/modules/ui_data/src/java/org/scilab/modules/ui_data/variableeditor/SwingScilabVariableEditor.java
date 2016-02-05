/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.ui_data.variableeditor;

import java.awt.Component;

import javax.swing.JButton;
import javax.swing.JMenu;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.TableModelEvent;
import javax.swing.table.TableModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.RowHeader;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.actions.CloseAction;
import org.scilab.modules.ui_data.variableeditor.actions.CopyAction;
import org.scilab.modules.ui_data.variableeditor.actions.CopySelectionAsScilabCommandAction;
import org.scilab.modules.ui_data.variableeditor.actions.CopySelectionAsScilabMatrixAction;
import org.scilab.modules.ui_data.variableeditor.actions.CopySelectionAsScilabMatrixWithCRAction;
import org.scilab.modules.ui_data.variableeditor.actions.CreateNewVariableAction;
import org.scilab.modules.ui_data.variableeditor.actions.CreateVariableFromSelectionAction;
import org.scilab.modules.ui_data.variableeditor.actions.CutAction;
import org.scilab.modules.ui_data.variableeditor.actions.DuplicateVariableAction;
import org.scilab.modules.ui_data.variableeditor.actions.ExportToCsvAction;
import org.scilab.modules.ui_data.variableeditor.actions.ExportToCsvFromSelectionAction;
import org.scilab.modules.ui_data.variableeditor.actions.PasteAction;
import org.scilab.modules.ui_data.variableeditor.actions.PlotAction;
import org.scilab.modules.ui_data.variableeditor.actions.RedoAction;
import org.scilab.modules.ui_data.variableeditor.actions.RefreshAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionLongAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionLongeAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionShortAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionShorteAction;
import org.scilab.modules.ui_data.variableeditor.actions.SizeColumnsToFitAction;
import org.scilab.modules.ui_data.variableeditor.actions.SupprAction;
import org.scilab.modules.ui_data.variableeditor.actions.UndoAction;
import org.scilab.modules.ui_data.variableeditor.celleditor.CellEditorFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;
import org.scilab.modules.ui_data.variableeditor.renderers.RendererFactory;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoManager;

/**
 * Swing implementation of Scilab Variable Editor
 * uses JTable
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class SwingScilabVariableEditor extends SwingScilabDockablePanel implements SimpleVariableEditor {

    /**
     * Prefix used in the tabs titles.
     */
    public static final String PREFIX = "Var - ";

    private static final String APOS = "'";
    private static final long serialVersionUID = 1L;

    private final ScilabTabbedPane tabPane;
    private final JButton refreshButton;
    private final JButton undoButton;
    private final JButton redoButton;

    /**
     * Create a JTable with data Model.
     * @param name the variable name
     * @param type the variable type
     * @param data : The JTable data.
     */
    public SwingScilabVariableEditor(String name, String type, Object[][] data) {
        super(UiDataMessages.VARIABLE_EDITOR);
        setAssociatedXMLIDForHelp("editvar");
        setWindowIcon("rrze_table");
        refreshButton = RefreshAction.createButton(this, UiDataMessages.REFRESH);
        undoButton = UndoAction.createButton(this, UiDataMessages.UNDO);
        redoButton = RedoAction.createButton(this, UiDataMessages.REDO);
        enableUndoButton(false);
        enableRedoButton(false);
        tabPane = new ScilabTabbedPane(this);
        tabPane.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                String name = tabPane.getScilabTitleAt(tabPane.getSelectedIndex());
                if (name.length() != 0) {
                    name = name.substring(PREFIX.length());
                    String tooltip = "";
                    if (name != null && getCurrentModel() != null) {
                        tooltip = UiDataMessages.REFRESH + APOS + name + APOS;
                        String type = ((SwingEditvarTableModel) getCurrentModel()).getType();
                        String title = UiDataMessages.VARIABLE_EDITOR + " - " + name + "  (" + type + ")";
                        setName(title);
                        SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, tabPane);
                        if (window != null) {
                            window.setTitle(title);
                        }
                    }
                    CellsUndoManager undoManager = ((SwingEditvarTableModel) getCurrentModel()).getUndoManager();
                    enableUndoButton(undoManager.canUndo());
                    enableRedoButton(undoManager.canRedo());
                    refreshButton.setToolTipText(tooltip);
                }
            }
        });
        setContentPane(tabPane);
        setData(name, type, data);
        WindowsConfigurationManager.restorationFinished(this);

        addToolBar(createToolBar());
        addMenuBar(createMenuBar());
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
     * @return the TabbedPane used in this window
     */
    public ScilabTabbedPane getTabPane() {
        return tabPane;
    }

    /**
     * @return the current table
     */
    public JTable getCurrentTable() {
        Component c = tabPane.getSelectedComponent();
        if (c != null) {
            return (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        }

        return null;
    }

    /**
     * @return the current table
     */
    public TableModel getCurrentModel() {
        JTable t = getCurrentTable();
        if (t != null) {
            return t.getModel();
        }

        return null;
    }

    /**
     * Create the toolbar
     * @return the built toolbar
     */
    public ToolBar createToolBar() {
        ToolBar toolBar = ScilabToolBar.createToolBar();
        SwingScilabToolBar stb = (SwingScilabToolBar) toolBar.getAsSimpleToolBar();

        stb.add(refreshButton);
        stb.addSeparator();
        stb.add(CreateVariableFromSelectionAction.createButton(this, UiDataMessages.CREATE));
        stb.add(CreateNewVariableAction.createButton(this, UiDataMessages.CREATENEWVAR));
        stb.addSeparator();
        stb.add(CutAction.createButton(this, UiDataMessages.CUT));
        stb.add(CopyAction.createButton(this, UiDataMessages.COPY));
        stb.add(PasteAction.createButton(this, UiDataMessages.PASTE));
        stb.addSeparator();
        stb.add(undoButton);
        stb.add(redoButton);
        stb.addSeparator();
        stb.add(SizeColumnsToFitAction.createButton(this, UiDataMessages.FIT));
        stb.addSeparator();
        stb.add(SetPrecisionShortAction.createButton(this, UiDataMessages.SHORT));
        stb.add(SetPrecisionShorteAction.createButton(this, UiDataMessages.SHORTE));
        stb.add(SetPrecisionLongAction.createButton(this, UiDataMessages.LONG));
        stb.add(SetPrecisionLongeAction.createButton(this, UiDataMessages.LONGE));
        stb.addSeparator();
        stb.add(PlotAction.createButton(this, UiDataMessages.PLOTSELECTION));

        return toolBar;
    }

    /**
     * @return a popup menu for the table
     */
    public JPopupMenu createPopupMenu() {
        JPopupMenu popup = new JPopupMenu();
        popup.setBorderPainted(true);
        popup.add(UndoAction.createJMenuItem(this, UiDataMessages.UNDO));
        popup.add(RedoAction.createJMenuItem(this, UiDataMessages.REDO));
        popup.addSeparator();
        popup.add(CutAction.createJMenuItem(this, UiDataMessages.CUT));
        popup.add(CopyAction.createJMenuItem(this, UiDataMessages.COPY));
        popup.add(PasteAction.createJMenuItem(this, UiDataMessages.PASTE));
        popup.addSeparator();
        popup.add(CopySelectionAsScilabCommandAction.createJMenuItem(this, UiDataMessages.COPYASSCILABCOM));
        popup.add(CopySelectionAsScilabMatrixAction.createJMenuItem(this, UiDataMessages.COPYASSCILABMAT));
        popup.add(CopySelectionAsScilabMatrixWithCRAction.createJMenuItem(this, UiDataMessages.COPYASSCILABMATWITHCR));
        popup.add(ExportToCsvFromSelectionAction.createJMenuItem(this, UiDataMessages.EXPORTSELECTIONCSV));
        popup.addSeparator();
        popup.add(CreateVariableFromSelectionAction.createJMenuItem(this, UiDataMessages.CREATE));
        popup.add(CreateNewVariableAction.createJMenuItem(this, UiDataMessages.CREATENEWVAR));
        popup.add(DuplicateVariableAction.createJMenuItem(this, UiDataMessages.DUPLICATE));
        popup.add(ExportToCsvAction.createJMenuItem(this, UiDataMessages.EXPORTCSV));

        popup.addSeparator();

        JMenu plots = new JMenu(UiDataMessages.PLOTSELECTION);
        popup.add(plots);

        plots.add(PlotAction.createJMenuItem(this, "plot2d", true));
        plots.add(PlotAction.createJMenuItem(this, "Matplot", true));
        plots.add(PlotAction.createJMenuItem(this, "grayplot", true));
        plots.add(PlotAction.createJMenuItem(this, "Sgrayplot", true));
        plots.add(PlotAction.createJMenuItem(this, "champ", true));
        plots.add(PlotAction.createJMenuItem(this, "histplot", true));
        plots.add(PlotAction.createJMenuItem(this, "mesh", true));
        plots.add(PlotAction.createJMenuItem(this, "surf", true));
        plots.add(PlotAction.createJMenuItem(this, "hist3d", true));
        plots.add(PlotAction.createJMenuItem(this, "contour2d", true));
        plots.add(PlotAction.createJMenuItem(this, "pie", true));

        popup.addSeparator();

        JMenu plotAll = new JMenu(UiDataMessages.PLOTALL);
        popup.add(plotAll);

        plotAll.add(PlotAction.createJMenuItem(this, "plot2d", false));
        plotAll.add(PlotAction.createJMenuItem(this, "Matplot", false));
        plotAll.add(PlotAction.createJMenuItem(this, "grayplot", false));
        plotAll.add(PlotAction.createJMenuItem(this, "Sgrayplot", false));
        plotAll.add(PlotAction.createJMenuItem(this, "champ", false));
        plotAll.add(PlotAction.createJMenuItem(this, "histplot", false));
        plotAll.add(PlotAction.createJMenuItem(this, "mesh", false));
        plotAll.add(PlotAction.createJMenuItem(this, "surf", false));
        plotAll.add(PlotAction.createJMenuItem(this, "hist3d", false));
        plotAll.add(PlotAction.createJMenuItem(this, "contour2d", false));
        plotAll.add(PlotAction.createJMenuItem(this, "pie", false));

        return popup;
    }

    public MenuBar createMenuBar() {
        MenuBar bar = ScilabMenuBar.createMenuBar();
        Menu file = ScilabMenu.createMenu();
        file.setText(UiDataMessages.FILE);
        file.add(CreateVariableFromSelectionAction.createMenuItem(this, UiDataMessages.CREATE));
        file.add(CreateNewVariableAction.createMenuItem(this, UiDataMessages.CREATENEWVAR));
        file.add(DuplicateVariableAction.createMenuItem(this, UiDataMessages.DUPLICATE));
        file.add(ExportToCsvAction.createMenuItem(this, UiDataMessages.EXPORTCSV));
        file.addSeparator();
        file.add(CloseAction.createMenuItem(this, UiDataMessages.CLOSE));

        bar.add(file);
        Menu edition = ScilabMenu.createMenu();
        edition.setText(UiDataMessages.EDITION);
        edition.add(UndoAction.createMenuItem(this, UiDataMessages.UNDO));
        edition.add(RedoAction.createMenuItem(this, UiDataMessages.REDO));
        edition.addSeparator();
        edition.add(CutAction.createMenuItem(this, UiDataMessages.CUT));
        edition.add(CopyAction.createMenuItem(this, UiDataMessages.COPY));
        edition.add(PasteAction.createMenuItem(this, UiDataMessages.PASTE));
        edition.addSeparator();
        edition.add(CopySelectionAsScilabCommandAction.createMenuItem(this, UiDataMessages.COPYASSCILABCOM));
        edition.add(CopySelectionAsScilabMatrixAction.createMenuItem(this, UiDataMessages.COPYASSCILABMAT));
        edition.add(CopySelectionAsScilabMatrixWithCRAction.createMenuItem(this, UiDataMessages.COPYASSCILABMATWITHCR));
        edition.addSeparator();
        edition.add(SizeColumnsToFitAction.createMenuItem(this, UiDataMessages.FIT));
        bar.add(edition);

        return bar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setData(String name, String type, Object[][] data) {
        TableVariableEditor table = new TableVariableEditor(this);

        CopyAction.registerAction(this, table);
        CutAction.registerAction(this, table);
        PasteAction.registerAction(this, table);
        SupprAction.registerAction(this, table);
        UndoAction.registerAction(this, table);
        RedoAction.registerAction(this, table);
        SizeColumnsToFitAction.registerAction(this, table);
        SetPrecisionShortAction.registerAction(this, table);
        SetPrecisionShorteAction.registerAction(this, table);
        SetPrecisionLongAction.registerAction(this, table);
        SetPrecisionLongeAction.registerAction(this, table);
        CreateVariableFromSelectionAction.registerAction(this, table);
        CreateNewVariableAction.registerAction(this, table);

        table.setComponentPopupMenu(createPopupMenu());

        ScilabGenericCellEditor cellEditor = (ScilabGenericCellEditor) CellEditorFactory.createCellEditor(type);
        int rows = Math.max(table.getMinimalRowNumber(), table.getModel().getRowCount());
        int cols = table.getModel().getColumnCount();

        SwingEditvarTableModel dataModel = new SwingEditvarTableModel(this, name, type, data, cellEditor, rows, cols);

        dataModel.addTableModelListener(getRowHeader(table));
        getRowHeader(table).tableChanged(new TableModelEvent(dataModel));
        table.setModel(dataModel);
        table.setDefaultEditor(Object.class, cellEditor);
        table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(type));

        tabPane.add(PREFIX + name, table.getScrollPane());
        tabPane.setSelectedComponent(table.getScrollPane());
    }

    /**
     * @param table the table to update
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix data
     */
    public void updateData(JTable table, String name, String type, Object[][] data) {
        ScilabGenericCellEditor cellEditor = (ScilabGenericCellEditor) CellEditorFactory.createCellEditor(type);
        int rows = Math.max(((TableVariableEditor) table).getMinimalRowNumber(), table.getModel().getRowCount());
        int cols = table.getModel().getColumnCount();
        CellsUndoManager undo = null;
        if (table.getModel() instanceof SwingEditvarTableModel) {
            undo = ((SwingEditvarTableModel) table.getModel()).getUndoManager();
        }

        SwingEditvarTableModel dataModel = (SwingEditvarTableModel) table.getModel();
        getRowHeader((TableVariableEditor) table).tableChanged(new TableModelEvent(dataModel));
        dataModel.changeData(type, data, cellEditor, rows, cols);
        table.setDefaultEditor(Object.class, cellEditor);
        table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(type));
    }

    /**
     * @param table the table to update
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(JTable table, String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex) {
        SwingEditvarTableModel dataModel = (SwingEditvarTableModel) table.getModel();
        if (!type.equals(dataModel.getType())) {
            dataModel.refresh();
        } else {
            for (int i = 0; i < rowsIndex.length; i++) {
                for (int j = 0; j < colsIndex.length; j++) {
                    int r = (int) rowsIndex[i] - 1;
                    int c = (int) colsIndex[j] - 1;
                    dataModel.setValueAtAndUpdate(false, false, data[i][j], r, c);
                }
            }
        }
    }

    /**
     * @param c the component in the tabbedpane
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix data
     */
    public void updateData(Component c, String name, String type, Object[][] data) {
        tabPane.setSelectedComponent(c);
        JTable table = (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        updateData(table, name, type, data);
    }

    /**
     * @param c the component in the tabbedpane
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(Component c, String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex) {
        tabPane.setSelectedComponent(c);
        JTable table = (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        updateData(table, name, type, data, rowsIndex, colsIndex);
    }

    /**
     * @return the row header used in this table
     */
    public RowHeader getRowHeader(TableVariableEditor table) {
        return (RowHeader) table.getScrollPane().getRowHeader().getView();
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * @param b true to enable the button
     */
    public void enableUndoButton(boolean b) {
        if (undoButton != null) {
            undoButton.setEnabled(b);
        }
    }

    /**
     * @param b true to enable the button
     */
    public void enableRedoButton(boolean b) {
        if (redoButton != null) {
            redoButton.setEnabled(b);
        }
    }
}
