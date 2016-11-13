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

import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JTable;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class CutAction extends CommonCallBack {

    private static final String KEY = "OSSCKEY X";
    private static final String CUT = "Cut";

    private final SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public CutAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(CUT, new CutAction(editor, CUT));
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), CUT);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
            table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);
            StringBuffer buf = new StringBuffer();
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

            for (int i = rows[0]; i <= rows[rows.length - 1]; i++) {
                for (int j = cols[0]; j <= cols[cols.length - 1]; j++) {
                    String val = model.getScilabValueAt(i, j, false);
                    if (val != null) {
                        buf.append(val);
                    }
                    model.emptyValueAt(i, j);
                    if (j < cols[cols.length - 1]) {
                        buf.append("\t");
                    }
                }
                buf.append("\n");
            }
            for (int i = rows[rows.length - 1]; i >= rows[0]; i--) {
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

            StringSelection sel = new StringSelection(buf.toString());
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(SwingScilabVariableEditor editor, String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new CutAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("edit-cut"));
        button.setIcon(imageIcon);

        return button;
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabVariableEditor editor, String title) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new CutAction(editor, title));
        menu.setText(title);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(ScilabKeyStroke.getKeyStroke(KEY));

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
