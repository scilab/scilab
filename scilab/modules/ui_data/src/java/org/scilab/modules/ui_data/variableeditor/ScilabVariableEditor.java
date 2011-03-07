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

import java.awt.Component;
import java.util.Map;
import java.util.WeakHashMap;

import javax.swing.SwingUtilities;
import javax.swing.event.ChangeListener;

import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoableEdit;

/**
 * Class ScilabVariableEditor
 * Implements a ScilabWindow containing Variable Editor (JTable)
 */
public final class ScilabVariableEditor extends ScilabWindow implements VariableEditor {

    private static final String MENUBARXMLFILE = System.getenv("SCI") + "/modules/ui_data/etc/variableeditor_menubar.xml";

    private static Map<String, Component> map = new WeakHashMap();

    private static VariableEditor instance;

    private static SimpleVariableEditor editorTab;
    private static ScilabTabbedPane tabPane;

    /**
     * Constructor
     * @param type the variable type
     * @param data the JTable data.
     * @param variableName the name in Scilab of the variable being edited.
     */
    private ScilabVariableEditor(String type, Object[][] data, String variableName) {
        super();
        editorTab = new SwingScilabVariableEditor(variableName, type, data);
        tabPane = ((SwingScilabVariableEditor) editorTab).getTabPane();
        editorTab.setCallback(ScilabCallBack.createCallback("org.scilab.modules.ui_data.EditVar.closeVariableEditor",
                                                            ScilabCallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE));
        MenuBar menubar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
        editorTab.addMenuBar(menubar);

        TextBox infobar = ScilabTextBox.createTextBox();
        editorTab.addInfoBar(infobar);
        addTab(editorTab);
        map.put(variableName, tabPane.getSelectedComponent());
    }

    /**
     * Close Variable Editor
     */
    public void close() {
        ScilabWindow editvarWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(editorTab.getParentWindowId());
        ChangeListener[] cl = tabPane.getChangeListeners();
        for (int i = 0; i < cl.length; i++) {
            tabPane.removeChangeListener(cl[i]);
        }
        editvarWindow.removeTab(editorTab);
        editorTab.setVisible(false);
        editorTab.close();
        instance = null;
        map.clear();
        CellsUndoableEdit.clear();
    }

    /**
     * Close the edition of the variable
     * @param name the variable
     */
    public static void close(String name) {
        map.remove(name);
        CellsUndoableEdit.removeVar(name);
    }

    /**
     * Set data displayed in JTable
     * @param name the variable name
     * @param type the variable type
     * @param data : data to be displayed in JTable
     */
    public void updateData(String name, String type, Object[][] data) {
        if (map.containsKey(name)) {
            ((SwingScilabVariableEditor) editorTab).updateData(map.get(name), name, type, data);
        } else {
            editorTab.setData(name, type, data);
            map.put(name, tabPane.getSelectedComponent());
        }
    }

    /**
     * Get the variable editor singleton
     * @return the Variable Editor
     */
    public static VariableEditor getVariableEditor() {
        return instance;
    }

    /**
     * Get the variable editor singleton with specified data.
     * @param type the variable type
     * @param data : the data to fill the editor with
     * @param variableName : the scilab name of the variable being edited.
     * @return the Variable Editor
     */
    public static VariableEditor getVariableEditor(final String type, final Object[][] data, final String variableName) {
        if (instance == null) {
            instance = new ScilabVariableEditor(type, data, variableName);
            instance.setVisible(true);
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingScilabVariableEditor) editorTab);
            window.setVisible(true);
            window.toFront();
            int row = -1;
            int col = -1;
            SwingEditvarTableModel model = (SwingEditvarTableModel) ((SwingScilabVariableEditor) editorTab).getCurrentModel();
            if (model != null) {
                row = model.getCurrentRow();
                col = model.getCurrentCol();
            }
            final int r = row;
            final int c = col;
            SwingUtilities.invokeLater(new Thread() {
                    public void run() {
                        instance.updateData(variableName, type, data);
                        if (r != -1 && c != -1) {
                            ((SwingScilabVariableEditor) editorTab).getCurrentTable().setRowSelectionInterval(r, r);
                            ((SwingScilabVariableEditor) editorTab).getCurrentTable().setColumnSelectionInterval(c, c);
                        }
                    }
                });
        }

        editorTab.setName(Messages.gettext("Variable Editor") + " - " + variableName + "  (" + type + ")");
        return instance;
    }

    /**
     * {@inheritDoc}
     */
    public String getVariablename() {
        String title = tabPane.getScilabTitleAt(tabPane.getSelectedIndex());
        return title.substring(SwingScilabVariableEditor.PREFIX.length());
    }

    /**
     * {@inheritDoc}
     */
    public void setVariableName(String variableName) {
        tabPane.setTitleAt(tabPane.getSelectedIndex(), SwingScilabVariableEditor.PREFIX + variableName);
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        super.setVisible(status);
        editorTab.setVisible(status);
    }
}
