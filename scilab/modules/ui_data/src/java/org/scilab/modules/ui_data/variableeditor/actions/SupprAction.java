/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.ui_data.variableeditor.actions;

import java.util.Vector;

import javax.swing.JTable;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * SupprAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class SupprAction extends CommonCallBack {

    private static final String KEY = "DELETE";
    private static final String SUPPR = "Suppr";

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public SupprAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(SUPPR, new SupprAction(editor, SUPPR));
        table.getInputMap().put(KeyStroke.getKeyStroke(KEY), SUPPR);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            Object oldValue;
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            int oldRows = model.getScilabMatrixRowCount();
            int oldCols = model.getScilabMatrixColCount();

            if (rows[0] >= oldRows || cols[0] >= oldCols) {
                return;
            }

            if ((cols.length == 1 && rows.length == 1) || (rows.length >= 2 && cols.length >= 2 && rows[1] >= oldRows && cols[1] >= oldCols)) {
                oldValue = model.getValueAt(rows[0], cols[0]);
            } else {
                oldValue = model.cloneDatas();
            }
            table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
            table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);
            for (int i = rows[rows.length - 1]; i >= rows[0]; i--) {
                for (int j = cols[cols.length - 1]; j >= cols[0]; j--) {
                    model.emptyValueAt(i, j);
                }
                model.removeRow(i, cols[0], cols[cols.length - 1]);
            }
            for (int j = cols[cols.length - 1]; j >= cols[0]; j--) {
                model.removeCol(j, rows[0], rows[rows.length - 1]);
            }

            if (oldValue instanceof Vector) {
                model.updateFullMatrix(oldValue, oldRows, oldCols);
            } else {
                model.updateMatrix(oldValue, rows[0], cols[0]);
            }
        }
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabVariableEditor editor, String title) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new SupprAction(editor, title));
        menu.setText(title);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(KeyStroke.getKeyStroke(KEY));

        return menu;
    }

    /**
     * Create a menu item as a SwingScilabMenuItem
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static SwingScilabMenuItem createJMenuItem(SwingScilabVariableEditor editor, String title) {
        return (SwingScilabMenuItem) createMenuItem(editor, title).getAsSimpleMenuItem();
    }
}
