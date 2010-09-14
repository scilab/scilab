/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
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
import java.awt.Component;

import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.table.TableModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.RowHeader;
import org.scilab.modules.ui_data.rowheader.RowHeaderModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.action.CopyAction;
import org.scilab.modules.ui_data.variableeditor.action.CutAction;
import org.scilab.modules.ui_data.variableeditor.action.PasteAction;
import org.scilab.modules.ui_data.variableeditor.action.RefreshAction;
import org.scilab.modules.ui_data.variableeditor.action.SupprAction;
import org.scilab.modules.ui_data.variableeditor.renderers.RendererFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.CellEditorFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;

/**
 * Swing implementation of Scilab Variable Editor
 * uses JTable
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class SwingScilabVariableEditor extends SwingScilabTab implements Tab, SimpleVariableEditor {

    /**
     * Prefix used in the tabs titles.
     */
    public static final String PREFIX = "Var - ";

    private static final String APOS = "'";
    private static final long serialVersionUID = 1L;

    private SwingEditvarTableModel dataModel;
    private JTabbedPane tabPane;
    private JScrollPane scrollPane;
    private PushButton refreshButton;

    /**
     * Create a JTable with data Model.
     * @param name the variable name
     * @param type the variable type
     * @param data : The JTable data.
     */
    public SwingScilabVariableEditor(String name, String type, Object[][] data) {
        super(UiDataMessages.VARIABLE_EDITOR);
        refreshButton = RefreshAction.createButton(this, UiDataMessages.REFRESH);
        tabPane = new JTabbedPane();
        tabPane.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    String name = tabPane.getTitleAt(tabPane.getSelectedIndex()).substring(PREFIX.length());
                    String tooltip = "";
                    if (name != null) {
                        tooltip = UiDataMessages.REFRESH + APOS + name + APOS;
                        String type = ((SwingEditvarTableModel) getCurrentModel()).getType();
                        String title = UiDataMessages.VARIABLE_EDITOR + " - " + name + "  (" + type + ")";
                        setName(title);
                        SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, tabPane);
                        if (window != null) {
                            window.setTitle(title);
                        }
                    }
                    refreshButton.setToolTipText(tooltip);
                }
            });
        setContentPane(tabPane);
        setData(name, type, data);
        ToolBar toolBar = ScilabToolBar.createToolBar();
        toolBar.add(refreshButton);
        toolBar.add(CutAction.createButton(this, UiDataMessages.CUT));
        toolBar.add(CopyAction.createButton(this, UiDataMessages.COPY));
        toolBar.add(PasteAction.createButton(this, UiDataMessages.PASTE));
        addToolBar(toolBar);
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
    public JTabbedPane getTabPane() {
        return tabPane;
    }

    /**
     * @return the current table
     */
    public JTable getCurrentTable() {
        return (JTable) ((JScrollPane) tabPane.getSelectedComponent()).getViewport().getComponent(0);
    }

    /**
     * @return the current table
     */
    public TableModel getCurrentModel() {
        return getCurrentTable().getModel();
    }

    /**
     * {@inheritDoc}
     */
    public void setData(String name, String type, Object[][] data) {
        JTable table = new JTable();
        CopyAction.registerAction(this, table);
        CutAction.registerAction(this, table);
        PasteAction.registerAction(this, table);
        SupprAction.registerAction(this, table);
        scrollPane = new JScrollPane(table);
        table.setFillsViewportHeight(true);
        table.setAutoResizeMode(CENTER);
        table.setRowHeight(25);

        updateData(table, type, data);

        table.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        table.setCellSelectionEnabled(true);

        table.setBackground(Color.WHITE);
        tabPane.add(PREFIX + name, scrollPane);
        tabPane.setSelectedComponent(scrollPane);
    }

    /**
     * @param table the table to update
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(JTable table, String type, Object[][] data) {
        ScilabGenericCellEditor cellEditor = (ScilabGenericCellEditor) CellEditorFactory.createCellEditor(type);
        if (table.getModel() instanceof SwingEditvarTableModel) {
            cellEditor.setExpressions(((SwingEditvarTableModel) table.getModel()).getCellEditor().getExpressions());
        }
        dataModel = new SwingEditvarTableModel(type, data, cellEditor);
        table.setModel(dataModel);
        RowHeaderModel rowHeaderModel = new RowHeaderModel(dataModel);
        RowHeader rowHeader = new RowHeader(rowHeaderModel, table);
        scrollPane.setRowHeaderView(rowHeader);
        table.setDefaultEditor(Object.class, cellEditor);
        table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(type));
    }

    /**
     * @param c the component in the tabbedpane
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(Component c, String type, Object[][] data) {
        tabPane.setSelectedComponent(c);
        JTable table = (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        updateData(table, type, data);
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
        return (Window) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
    }
}
