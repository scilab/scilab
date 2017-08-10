/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
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
import java.util.HashMap;
import java.util.Map;

import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeListener;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.bridge.window.SwingScilabDockingWindow;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.localization.Messages;

/**
 * Class ScilabVariableEditor
 * Implements a ScilabWindow containing Variable Editor (JTable)
 */
@SuppressWarnings(value = { "serial" })
public final class ScilabVariableEditor extends SwingScilabDockingWindow implements VariableEditor {

    private static Map<String, Component> map = new HashMap();

    private static ScilabVariableEditor instance;

    private static SwingScilabVariableEditor editorTab;
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
        tabPane = editorTab.getTabPane();
        TextBox infobar = ScilabTextBox.createTextBox();
        editorTab.addInfoBar(infobar);
        addTab(editorTab);
        map.put(variableName, tabPane.getSelectedComponent());
    }

    /**
     * Close Variable Editor
     */
    public void close() {
        SwingScilabWindow editvarWindow = SwingScilabWindow.allScilabWindows.get(editorTab.getParentWindowId());
        ChangeListener[] cl = tabPane.getChangeListeners();
        for (int i = 0; i < cl.length; i++) {
            tabPane.removeChangeListener(cl[i]);
        }
        instance = null;
        map.clear();
        super.close();
    }

    /**
     * Close the edition of the variable
     */
    public static void closeEditVar() {
        if (instance != null) {
            instance.close();
        }
    }

    /**
     * Close the edition of the variable
     * @param name the variable
     */
    public static void close(String name) {
        map.remove(name);
    }

    /**
     * Set data displayed in JTable
     * @param name the variable name
     * @param type the variable type
     * @param data : data to be displayed in JTable
     */
    public void updateData(String name, String type, Object[][] data) {
        if (map.containsKey(name)) {
            editorTab.updateData(map.get(name), name, type, data);
        } else {
            editorTab.setData(name, type, data);
            map.put(name, tabPane.getSelectedComponent());
        }
    }

    /**
     * Set data displayed in JTable
     * @param name the variable name
     * @param type the variable type
     * @param data : data to be displayed in JTable
     */
    public void updateData(String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex) {
        if (map.containsKey(name)) {
            editorTab.updateData(map.get(name), name, type, data, rowsIndex, colsIndex);
        }
    }

    /**
     * Get the variable editor singleton
     * @return the Variable Editor
     */
    public static ScilabVariableEditor getVariableEditor() {
        return instance;
    }

    /**
     * Get the variable editor singleton with specified data.
     * @param type the variable type
     * @param data : the data to fill the editor with
     * @param variableName : the scilab name of the variable being edited.
     * @return the Variable Editor
     */
    public static ScilabVariableEditor getVariableEditor(final String type, final Object[][] data, final String variableName) {
        if (instance == null) {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        instance = new ScilabVariableEditor(type, data, variableName);
                        instance.setVisible(true);
                    }
                });
            } catch (Exception e) {
                System.err.println(e);
            }
        } else {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, editorTab);
                    window.setVisible(true);
                    window.toFront();
                    final JTable table = editorTab.getCurrentTable();
                    int r = -1;
                    int c = -1;

                    if (table != null) {
                        r = table.getSelectedRow();
                        c = table.getSelectedColumn();
                    }
                    boolean b = map.containsKey(variableName);
                    instance.updateData(variableName, type, data);
                    if (b && r != -1 && c != -1) {
                        table.setRowSelectionInterval(r, r);
                        table.setColumnSelectionInterval(c, c);
                    }
                }
            });
        }

        editorTab.setName(Messages.gettext("Variable Editor") + " - " + variableName + "  (" + type + ")");
        return instance;
    }

    public static void refreshVariableEditor(final String type, final Object[][] data, double[] rowsIndex, double[] colsIndex, final String variableName) {
        if (instance != null) {
            instance.updateData(variableName, type, data, rowsIndex, colsIndex);
        }
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

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X "About" menu is called. It is the only case where this method
     * should be used
     */
    public void macosxAbout() {
        InterpreterManagement.requestScilabExec("about()");
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X "Quit Scilab" menu is called. It is the only case where this method
     * should be used
     */
    public boolean macosxQuit() {
        InterpreterManagement.requestScilabExec("exit()");
        return false;
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X "Preferences" menu is called. It is the only case where this method
     * should be used
     */
    public void macosxPreferences() {
        InterpreterManagement.requestScilabExec("preferences();");
    }


}
